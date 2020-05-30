// Copyright 2020, Nathan Blane

#include "Graphics/GraphicsInterface.hpp"
#include "Camera/Camera.h"
#include "Camera/CameraManager.h"
#include "Mesh/MeshManager.h"
#include "Input/Input.hpp"
#include "Texture/Texture2D/TextureManager.h"

#include "Scene/GameWorld.hpp"
#include "Scene/Scene.hpp"

#include "GameObject/GameObjectManager.h"
#include "GameObject/GameObject.h"
#include "Lighting/SpotLight.hpp"
#include "Shader/Material.h"
#include "Model/ModelFactory.h"
#include "Camera/GodCamera.hpp"

#include "Engine/FrameData.hpp"
#include "Engine/Internal/FrameDataInternal.hpp"

#include "Shader/ShaderDefinition.hpp"
#include "Shader/ShaderObjects/UnlitShading.hpp"
#include "Shader/ShaderObjects/BlinnShading.hpp"

#include "Debugging/MetricInterface.hpp"
#include "Debugging/ProfilerStatistics.hpp"

#include "RenderPipeline/UserInterfacePipeline.hpp"
#include "RenderPipeline/DeferredRenderPipeline.hpp"

#include "Font/FontCache.hpp"

#include "MusaEngine.hpp"
#include "Entry/MusaApp.hpp"

// TODO - Probably should delete this
#include "Graphics/RenderContextUtilities.hpp"
#include "Graphics/RenderContext.hpp"

// NOTE - This is for deserializing PAK files.
// TODO - The way pak files are deserialized, or even structured, needs to either change or be reexampined
#include "Archiver/FileDeserializer.hpp"
#include "Archiver/PackageHeader.h"
#include "Archiver/ChunkHeader.h"
#include "Archiver/AnimationHeader.h"
#include "Model/ModelFileHeader.h"
#include "Animation/AnimationClipFactory.h"
#include "Animation/Clip.h"
#include "Animation/Skeleton/SkeletonManager.h"
#include "Animation/Skeleton/Skeleton.h"
#include "DirectoryLocations.h"

#include "Memory/MemoryCore.hpp"

GLOBAL_OPERATOR_NEW_DELETE_OVERLOADS


DECLARE_METRIC_GROUP(Engine);
DECLARE_METRIC_GROUP(FrameRender);

METRIC_STAT(UpdateAndRender, Engine);
METRIC_STAT(Update, Engine);
METRIC_STAT(Render, Engine);

METRIC_STAT(BeginRenderFrame, FrameRender)
METRIC_STAT(EndRenderFrame, FrameRender)

static void InitializeGBuffer(GBuffer& gbuffer, u32 width, u32 height)
{
	if (!gbuffer.positionTexture)
	{
		gbuffer.positionTexture = CreateRenderTarget(
			ImageFormat::RGBA_16f,
			width, height,
			LoadOperation::Clear, StoreOperation::Store,
			LoadOperation::DontCare, StoreOperation::DontCare,
			TextureUsage::RenderTarget
		);
	}
	if (!gbuffer.normalTexture)
	{
		gbuffer.normalTexture = CreateRenderTarget(
			ImageFormat::RGBA_16f,
			width, height,
			LoadOperation::Clear, StoreOperation::Store,
			LoadOperation::DontCare, StoreOperation::DontCare,
			TextureUsage::RenderTarget
		);
	}
	if (!gbuffer.diffuseTexture)
	{
		gbuffer.diffuseTexture = CreateRenderTarget(
			ImageFormat::RGBA_8norm,
			width, height,
			LoadOperation::Clear, StoreOperation::Store,
			LoadOperation::DontCare, StoreOperation::DontCare,
			TextureUsage::RenderTarget
		);
	}
}

static void InitializeSceneTargets(SceneRenderTargets& sceneTargets, u32 width, u32 height)
{
	if (!sceneTargets.sceneColorTexture)
	{
		sceneTargets.sceneColorTexture = CreateRenderTarget(
			ImageFormat::RGBA_8norm,
			width, height,
			LoadOperation::Clear, StoreOperation::Store,
			LoadOperation::DontCare, StoreOperation::DontCare,
			TextureUsage::RenderTarget
		);
	}
	if (!sceneTargets.depthTexture)
	{
		sceneTargets.depthTexture = CreateRenderTarget(
			ImageFormat::DS_32f_8u,
			width, height,
			LoadOperation::Clear, StoreOperation::Store,
			LoadOperation::DontCare, StoreOperation::DontCare,
			TextureUsage::DepthStencilTarget
		);
	}
}

static void InitializeUITarget(UniquePtr<RenderTarget>& uiTarget, u32 width, u32 height)
{
	if (!uiTarget)
	{
		uiTarget = CreateRenderTarget(
			ImageFormat::RGBA_8norm,
			width, height,
			LoadOperation::Clear, StoreOperation::Store,
			LoadOperation::DontCare, StoreOperation::DontCare,
			TextureUsage::RenderTarget
		);
	}
}

static void InitializeFrameRenderTargets(FrameRenderTargets& renderTargets, const Viewport& viewport)
{
	u32 width = viewport.GetWidth(), height = viewport.GetHeight();
	InitializeGBuffer(renderTargets.gbuffer, width, height);
	InitializeSceneTargets(renderTargets.sceneTargets, width, height);
	InitializeUITarget(renderTargets.userInterfaceTarget, width, height);
}

MusaEngine::MusaEngine(UI::Context& context)
	: uiContext(&context)
{
	gameInput = MakeUnique<GameInput>(*this);
}

void MusaEngine::StartupEngine(Window& window)
{
	SetupWindowContext(window);
	InitializeSceneView();
}

void MusaEngine::ShutdownEngine()
{
	gameInput.Reset();
	world.Reset();
	viewport.Reset();
}

void MusaEngine::SetupWindowContext(Window& window)
{
	world = MakeUnique<GameWorld>(window);
	viewport = MakeUnique<Viewport>(window.GetWindowHandle(), window.GetWidth(), window.GetHeight());
}

void MusaEngine::InitializeSceneView()
{
	// TODO - This should be available here, so the width and height should be removed
	const i32 width = viewport->GetWidth();
	const i32 height = viewport->GetHeight();

	const f32 aspect = (f32)width / (f32)height;
	const IntRect viewportDim = { 0, 0, width, height };

	Camera* mainCamera = new Camera;
	mainCamera->SetViewport(viewportDim);
	mainCamera->SetPerspective(60.f, aspect, .1f, 10000.f);
	mainCamera->SetOrientationAndPosition(Vector4(0, 0, 0), Vector4(0, 0, 10.f), Vector4(0, 1, 0));

	GetCameraManager().AddCamera(mainCamera, "Main Camera");
	GetCameraManager().SetActiveCamera("Main Camera");

	GodCamera* godCam = world->CreateGameObject<GodCamera>(*mainCamera);
	gameInput->RegisterInputCallback(std::bind(&GodCamera::InputCallback, godCam, std::placeholders::_1));
}

void MusaEngine::StartRunningEngine()
{
	running = true;
}

void MusaEngine::StopRunningEngine()
{
	running = false;
}

static void CreateInputContext(GameInput& gameInput)
{
	PlayerInputContext mainContext = MakeInputContext("Keyboard Context");
	PlayerInputContext gamepadContext = MakeInputContext("Gamepad Context");
	RangedInput mouseXInput;
	mouseXInput.range = {
		-10, 10, -1, 1
	};
	mouseXInput.input = {
		"Mouse X",
		Inputs::Mouse_XAxis
	};
	mainContext.inputRanges.Add(mouseXInput);

	RangedInput mouseYInput;
	mouseYInput.range = {
		-10, 10, -1, 1
	};
	mouseYInput.input = {
		"Mouse Y",
		Inputs::Mouse_YAxis
	};
	mainContext.inputRanges.Add(mouseYInput);

	SingleInput input = {
		"Move Left",
		Inputs::Key_A
	};
	mainContext.inputStates.Add(input);

	input = {
		"Move Right",
		Inputs::Key_D
	};
	mainContext.inputStates.Add(input);

	input = {
		"Move Forward",
		Inputs::Key_W
	};
	mainContext.inputStates.Add(input);

	input = {
		"Move Backward",
		Inputs::Key_S
	};
	mainContext.inputStates.Add(input);

	input = {
	"Move Up",
	Inputs::Key_E
	};
	mainContext.inputStates.Add(input);

	input = {
		"Move Down",
		Inputs::Key_Q
	};
	mainContext.inputStates.Add(input);

	input = {
		"Space",
		Inputs::Key_Space
	};
	mainContext.inputActions.Add(input);


	// Controller support

	RangedInput leftXInput;
	leftXInput.range = {
		-100, 100, -1, 1
	};
	leftXInput.input = {
		"Leftstick X",
		Inputs::Gamepad_LeftStick_XAxis
	};
	gamepadContext.inputRanges.Add(leftXInput);

	RangedInput leftYInput;
	leftYInput.range = {
		-100, 100, -1, 1
	};
	leftYInput.input = {
		"Leftstick Y",
		Inputs::Gamepad_LeftStick_YAxis
	};
	gamepadContext.inputRanges.Add(leftYInput);

	RangedInput rightXInput;
	rightXInput.range = {
		-100, 100, -1, 1
	};
	rightXInput.input = {
		"Rightstick X",
		Inputs::Gamepad_RightStick_XAxis
	};
	gamepadContext.inputRanges.Add(rightXInput);

	RangedInput rightYInput;
	rightYInput.range = {
		-100, 100, -1, 1
	};
	rightYInput.input = {
		"Rightstick Y",
		Inputs::Gamepad_RightStick_YAxis
	};
	gamepadContext.inputRanges.Add(rightYInput);

	RangedInput leftTriggerInput = {};
	leftTriggerInput.range = {
		0, 1, 0, 1
	};
	leftTriggerInput.input = {
		"Left Trigger",
		Inputs::Gamepad_LeftTrigger
	};
	gamepadContext.inputRanges.Add(leftTriggerInput);

	RangedInput rightTriggerInput = {};
	rightTriggerInput.range = {
		0, 1, 0, 1
	};
	rightTriggerInput.input = {
		"Right Trigger",
		Inputs::Gamepad_RightTrigger
	};
	gamepadContext.inputRanges.Add(rightTriggerInput);

	input = {
	"Gamepad A",
	Inputs::Gamepad_AButton
	};
	gamepadContext.inputActions.Add(input);

	gameInput.AddInputContext(mainContext);
	gameInput.AddInputContext(gamepadContext);
	gameInput.PushInputContext("Keyboard Context");
	gameInput.PushInputContext("Gamepad Context");
}

// TODO - This SHOULD NOT exist in the engine file. It should exist in the file reading system probably...
static void LoadPakFile(const Path& pakPath)
{
	FileDeserializer pakFile(pakPath);
	PackageHeader pakHeader;
	Deserialize(pakFile, pakHeader);
	
	for (u32 i = 0; i < pakHeader.numChunks; ++i)
	{
		ChunkHeader chunkHeader;
		Deserialize(pakFile, chunkHeader);

		switch (chunkHeader.type)
		{
			case Chunk::VERTS_TYPE:
			{
				// TODO - This is sort of gross...there has to be a way to reduce the memory allocation. Allocating from the deserialization
				// and then one from the mesh manager
				// TODO - Deserialize the header here
				u8* modelData = new u8[chunkHeader.chunkSize];
				pakFile.DeserializeData(modelData, chunkHeader.chunkSize);
				
				GetMeshManager().LoadMeshFromPak(modelData, chunkHeader.chunkName);
			}break;
			case Chunk::TEXTURE_TYPE:
			{
				// TODO - Need to allow the Serialization/Deserialization functions to work with this
				MemoryBuffer texData(chunkHeader.chunkSize);
				pakFile.DeserializeData(texData.GetData(), chunkHeader.chunkSize);

				GetTextureManager().LoadTexture(texData, chunkHeader.chunkName);
			}break;
			case Chunk::SKEL_TYPE:
			{
				MemoryBuffer skelData(chunkHeader.chunkSize);
				pakFile.DeserializeData(skelData.GetData(), skelData.Size());

				NOT_USED Skeleton* skeleton = SkeletonManager::CreateSkeleton(skelData.GetData(), chunkHeader.hashNum);
			}
			case Chunk::WEIGHTS_TYPE:
			{
				// TODO - Weights need to be stored with the mesh, at this point, but this sort of thing needs to be revisited...
				MemoryBuffer weights(chunkHeader.chunkSize);
				pakFile.DeserializeData(weights.GetData(), weights.Size());

			}
			case Chunk::ANIM_TYPE:
			{
				MemoryBuffer animData(chunkHeader.chunkSize);
				pakFile.DeserializeData(animData.GetData(), chunkHeader.chunkSize);
				
				AnimationClip* clip = AnimationClipFactory::CreateAnimationClip(animData.GetData());
				Skeleton* skeleton = SkeletonManager::FindSkeleton(clip->GetSkeletonHash());
				skeleton->AddAnimation(clip);
			}break;
			default:
				break;
		}

	}
}

// TODO - This will eventually be moved. Mostly for quick turn around
#include "UI/DebugUI/UIConsole.hpp"
#include "GameObject/DemoGameObjects/OrbitOtherObject.hpp"
#include "GameObject/DemoGameObjects/MoveBetweenObject.hpp"
#include "GameObject/DemoGameObjects/ScaleChangingObject.hpp"

#include "Platform/PlatformMemory.hpp"

void MusaEngine::LoadContent()
{
	NOT_USED Memory::PlatformMemoryInfo info = Memory::GetPlatformMemoryInfo();
	// TODO - This shouldn't be in the load content function as it stands. However, it will be in some sort of load defaults 
	GetTextureManager().AddTexture(*WhiteTexture());
	GetTextureManager().AddTexture(*BlackTexture());

	GetMeshManager().Initialize();

	Path bunnyPakPath(EngineAssetPath() + "Models/stanford-bunny.pak");
	LoadPakFile(bunnyPakPath);

	Path astroPakPath(EngineAssetPath() + "Models/astro-boy.pak");
	LoadPakFile(astroPakPath);

	Path gethPakPath(EngineAssetPath() + "Models/geth-trooper.pak");
	LoadPakFile(gethPakPath);

	CreateInputContext(*gameInput);
	gameInput->LockCusorToView(true);
	gameInput->ShowCursor(false);

	ImportTTFont(Path("C:\\Windows\\Fonts\\Arial.ttf"));

	NOT_USED Mesh* box = GetMeshManager().FindMesh(Mesh::BoxName);
	NOT_USED Mesh* bunny = GetMeshManager().FindMesh("stanford-bunny");
	NOT_USED Mesh* astro_boy = GetMeshManager().FindMesh("astro-boy");
	NOT_USED Mesh* geth = GetMeshManager().FindMesh("geth-trooper");

	NativeVertexShader& vertShader = GetShader<UnlitVert>()->GetNativeShader();
	NativeFragmentShader& fragShader = GetShader<UnlitFrag>()->GetNativeShader();

	GameObject* gethObject = world->CreateGameObject<GameObject>();
	gethObject->SetModel(ModelFactory::CreateModel(geth, new Material(vertShader, fragShader, "ME3_360_ENEMY_Geth_Trooper_Body_D_t0", Color32::White())));

	GameObject* go = world->CreateGameObject<OrbitOtherObject>(*gethObject, 5.5f, Vector4::RightAxis);
	go->SetModel(ModelFactory::CreateModel(astro_boy, new Material(vertShader, fragShader, "astroboy_t0", Color32::White())));
	go->SetPos(Vector4(-1.5f, 2.5f, 0));
	
	go = world->CreateGameObject<OrbitOtherObject>(*go, 2.f, Vector4::UpAxis);
	go->SetModel(ModelFactory::CreateModel(box, new Material(vertShader, fragShader, WhiteTexture(), Color32::Blue())));
	go->SetPos(Vector4(5, 0, 0));

// 	constexpr uint32 numDoubleObjects = 400;
// 
// 	Vector4 pointA(-200.f, 2.f, -1.f);
// 	Vector4 pointB(-200.f, 4.f, -1.f);
// 
// 	Vector4 scalePos(-400, 5, 7.f);
// 	float dir = 1;
// 	Color32 color = Color32::Blue();
// 	for (uint32 i = 0; i < numDoubleObjects; ++i)
// 	{
// 		Vector4 tmp = pointA;
// 		pointA = pointB;
// 		pointB = tmp;
// 
// 		go = world->CreateGameObject<MoveBetweenObject>(pointA, pointB, 1.f);
// 		go->SetModel(ModelFactory::CreateModel(box, new Material(vertShader, fragShader, WhiteTexture(), color)));
// 
// 		pointA.x += 1.5f;
// 		pointB.x += 1.5f;
// 
// 		constexpr float32 scaleDelta = 2.f;
// 		if (dir < 0)
// 		{
// 			go = world->CreateGameObject<ScaleChangingObject>(1.f, 3.f, scaleDelta);
// 			color = Color32::Blue();
// 		}
// 		else
// 		{
// 			go = world->CreateGameObject<ScaleChangingObject>(3.f, 1.f, scaleDelta);
// 			color = Color32::Red();
// 		}
// 		go->SetModel(ModelFactory::CreateModel(box, new Material(vertShader, fragShader, WhiteTexture(), color)));
// 		go->SetPos(scalePos);
// 		
// 		scalePos.y *= dir;
// 		scalePos.x += 2.5f;
// 		dir *= -1.f;
// 	}
// 
// 	pointA.y -= 5.f;
// 	pointB.y -= 5.f;
// 	scalePos.y = -5;
// 	for (uint32 i = 0; i < numDoubleObjects; ++i)
// 	{
// 		Vector4 tmp = pointA;
// 		pointA = pointB;
// 		pointB = tmp;
// 
// 		go = world->CreateGameObject<MoveBetweenObject>(pointA, pointB, 1.f);
// 		go->SetModel(ModelFactory::CreateModel(box, new Material(vertShader, fragShader, WhiteTexture(), color)));
// 
// 		pointA.x -= 1.5f;
// 		pointB.x -= 1.5f;
// 
// 		constexpr float32 scaleDelta = 2.f;
// 		if (dir < 0)
// 		{
// 			go = world->CreateGameObject<ScaleChangingObject>(1.f, 3.f, scaleDelta);
// 			color = Color32::Blue();
// 		}
// 		else
// 		{
// 			go = world->CreateGameObject<ScaleChangingObject>(3.f, 1.f, scaleDelta);
// 			color = Color32::Red();
// 		}
// 		go->SetModel(ModelFactory::CreateModel(box, new Material(vertShader, fragShader, WhiteTexture(), color)));
// 		go->SetPos(scalePos);
// 
// 		scalePos.y *= dir;
// 		scalePos.x -= 2.5f;
// 		dir *= -1.f;
// 	}


	// TODO - LEAKING MEMEORY!!!
	//UI::Console* console = new UI::Console;
	//uiContext->AddWidget(*console);
}

void MusaEngine::UnloadContent()
{
	GetMeshManager().Deinitialize();
	GetTextureManager().Deinitialize();
}

void MusaEngine::UpdateAndRender(f32 tick)
{
	SCOPED_TIMED_BLOCK(UpdateAndRender);

	BEGIN_TIMED_BLOCK(Update);
	world->TickWorld(tick);
	uiContext->Update();

	world->PushToRenderState();
	END_TIMED_BLOCK(Update);

	BEGIN_TIMED_BLOCK(Render);
	RenderFrame();
	END_TIMED_BLOCK(Render);
}

void MusaEngine::GatherFrameMetrics()
{
	{
		//SCOPED_TIMED_BLOCK(GatherMetrics);
		GetProfilingStatistics().CollectAllFrameMetrics();
	}
	GetProfilingStatistics().ProfileFrameIncrement();
}

void MusaEngine::RenderFrame()
{
	InitializeFrameRenderTargets(engineTargets, *viewport);

	RenderContext& context = *GetGraphicsInterface().GetRenderContext();
	ScreenView& screenView = world->GetView();
	RenderObjectManager& renderManager = world->GetRenderManager();

	BEGIN_TIMED_BLOCK(BeginRenderFrame);
	context.BeginRenderFrame(viewport->GetNativeViewport());
	END_TIMED_BLOCK(BeginRenderFrame);

	//world->RenderWorld(engineTargets.gbuffer, engineTargets.sceneTargets, *engineTargets.userInterfaceTarget, *viewport);
	DeferredRender::Render(engineTargets, world->GetScene(), renderManager, screenView.view);
	DeferredRender::RenderUI(context, *uiContext, *engineTargets.userInterfaceTarget, screenView.view);

	// Need a way to compose everything to the backbuffer
	DeferredRender::ComposeBackbuffer(context, *engineTargets.sceneTargets.sceneColorTexture, *engineTargets.userInterfaceTarget, screenView.view);

	BEGIN_TIMED_BLOCK(EndRenderFrame);
	context.EndRenderFrame(viewport->GetNativeViewport());
	END_TIMED_BLOCK(EndRenderFrame);
}

