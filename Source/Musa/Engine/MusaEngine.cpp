
#include "Graphics/GraphicsInterface.hpp"
#include "Camera/Camera.h"
#include "Camera/CameraManager.h"
#include "Math/Rect.hpp"
#include "Types/Intrinsics.hpp"
#include "Math/Vector4.h"
#include "Mesh/MeshManager.h"
#include "Window/Window.h"
#include "Input/Input.hpp"
#include "Texture/Texture2D/TextureManager.h"
#include "DirectoryLocations.h"

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

#include "Font/FontCache.hpp"

#include "MusaEngine.hpp"
#include "Entry/MusaApp.hpp"

DECLARE_METRIC_GROUP(Engine);

METRIC_STAT(UpdateAndRender, Engine);
METRIC_STAT(Update, Engine);
METRIC_STAT(Render, Engine);
//METRIC_STAT(GatherMetrics, Engine);

MusaEngine::MusaEngine(GameUIContext& context)
	: uiContext(&context)
{
	gameInput = MakeUnique<GameInput>(*this);
}

void MusaEngine::InitializeGraphics()
{
	GetGraphicsInterface().InitializeGraphics();

	InitializeShaders();
}

void MusaEngine::SetupWindowContext(Window& window)
{
	world = MakeUnique<GameWorld>(window);
	viewport = MakeUnique<Viewport>(window.GetWindowHandle(), window.GetWidth(), window.GetHeight());

	GetGameObjectManager().Initialize(*world);

	InitializeInput();
}

void MusaEngine::InitializeSceneView()
{
	// TODO - This should be available here, so the width and height should be removed
	const int32 width = viewport->GetWidth();
	const int32 height = viewport->GetHeight();

	const float32 aspect = (float32)width / (float32)height;
	const IntRect viewportDim = { 0, 0, width, height };

	Camera* mainCamera = new Camera;
	mainCamera->SetViewport(viewportDim);
	mainCamera->SetPerspective(60.f, aspect, .1f, 10000.f);
	mainCamera->SetOrientationAndPosition(Vector4(0, 0, 0), Vector4(0, 0, 155.f), Vector4(0, 1, 0));

	GetCameraManager().AddCamera(mainCamera, "Main Camera");
	GetCameraManager().SetActiveCamera("Main Camera");

	GodCamera* godCam = GetGameObjectManager().CreateAndAdd<GodCamera>(*mainCamera);
	AddInputCallback(std::bind(&GodCamera::InputCallback, godCam, std::placeholders::_1));
}

void MusaEngine::RunEngine()
{
	running = true;


	LoadContent();


	world.Release();
	GetTextureManager().Deinitialize();
}

void MusaEngine::StartEngine()
{
	running = true;
}

void MusaEngine::StopEngine()
{
	running = false;
}

static void CreateInputContext()
{
	InputContext mainContext = MakeInputContext("Main Context");
	RangedInput mouseXInput;
	mouseXInput.range = {
		-100, 100, -1, 1
	};
	mouseXInput.input = {
		"Mouse X",
		Inputs::Mouse_XAxis
	};
	mainContext.inputRanges.Add(mouseXInput);

	RangedInput mouseYInput;
	mouseYInput.range = {
		-100, 100, -1, 1
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

	AddInputContext(mainContext);
	PushInputContext("Main Context");
}

void MusaEngine::LoadContent()
{
	// TODO - This shouldn't be in the load content function as it stands. However, it will be in some sort of load defaults 
	GetTextureManager().AddTexture(*WhiteTexture());
	GetTextureManager().AddTexture(*BlackTexture());

	GetMeshManager().LoadPrimitive(Primitive::Box);
	GetMeshManager().LoadPrimitive(Primitive::Plane);
	GetMeshManager().LoadPrimitive(Primitive::Sphere);
	GetMeshManager().LoadPrimitive(Primitive::Pyramid);

	CreateInputContext();

	ImportTTFont(Path("C:\\Windows\\Fonts\\Arial.ttf"));

	Mesh* sphere = GetMeshManager().FindMesh(Mesh::BoxName);

	ShaderResource& vertShader = GetShader<UnlitVert>()->GetNativeShader();
	ShaderResource& fragShader = GetShader<UnlitFrag>()->GetNativeShader();

	GameObject* go = GetGameObjectManager().CreateAndAdd<GameObject>();
	go->SetModel(ModelFactory::CreateModel(sphere, new Material(vertShader, fragShader, "Ariel", Color32::White())));
	go->SetScale(30, 30, 30);

	// TODO - A scene currently requires a light in it. This is very bad. It needs to be so lighting is done in a separate pass and then the screen shader renders to the screen...
	Light* light = GetGameObjectManager().CreateAndAdd<SpotLight>();
	light->SetPos(Vector4(100, 100, 100));
	light->SetRotation(-40.f, 45.f, 0.f);
	world->GetScene().AddLightToScene(*light);
}

void MusaEngine::UpdateAndRenderWorld(float32 tick)
{
	SCOPED_TIMED_BLOCK(UpdateAndRender);

	BEGIN_TIMED_BLOCK(Update);
	world->TickWorld(tick);
	world->PushToRenderState();
	END_TIMED_BLOCK(Update);

	BEGIN_TIMED_BLOCK(Render);
	world->RenderWorld(*viewport);
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

