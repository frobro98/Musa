
#include "Graphics/GraphicsInterface.hpp"
#include "Camera/Camera.h"
#include "Camera/CameraManager.h"
#include "Math/Rect.hpp"
#include "Types/Intrinsics.hpp"
#include "Math/Vector4.h"
#include "Mesh/MeshManager.h"
#include "Windowing/Window.h"
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

#include "Shader/ShaderDefinition.hpp"
#include "Shader/ShaderObjects/UnlitShading.hpp"
#include "Shader/ShaderObjects/BlinnShading.hpp"

#include "Debugging/MetricInterface.hpp"
#include "Debugging/ProfilerStatistics.hpp"

#include "Font/FontCache.hpp"

#include "GameEngine.hpp"

DECLARE_METRIC_GROUP(Engine);

METRIC_STAT(UpdateAndRender, Engine);
METRIC_STAT(Update, Engine);
METRIC_STAT(Render, Engine);
METRIC_STAT(GatherMetrics, Engine);

void GameEngine::RunEngine()
{
	int32 width = 1080;
	int32 height = 720;

	// TODO - This call needs some sort of type safety for the window handle...
	GetGraphicsInterface().InitializeGraphics();

	InitializeShaders();

	Window* window = new Window(width, height);
	window->Initialize();
	window->SetAsActiveWindow();

	world = new GameWorld(*window);

	GetGameObjectManager().Initialize(*world);

	//GetInputManager().Initialize(*window);
	InitializeInput(*window);

	const float32 aspect = (float32)width / (float32)height;
	const IntRect viewport = { 0, 0, width, height };

	Camera* mainCamera = new Camera;
	mainCamera->SetViewport(viewport);
	mainCamera->SetPerspective(60.f, aspect, .1f, 10000.f);
	mainCamera->SetOrientationAndPosition(Vector4(0, 0, 0), Vector4(0, 0, 155.f), Vector4(0, 1, 0));

	GetCameraManager().AddCamera(mainCamera, "Main Camera");
	GetCameraManager().SetActiveCamera("Main Camera");

	GetTextureManager().AddTexture(*WhiteTexture());
	GetTextureManager().AddTexture(*BlackTexture());

	GetMeshManager().LoadPrimitive(Primitive::Box);
	GetMeshManager().LoadPrimitive(Primitive::Plane);
	GetMeshManager().LoadPrimitive(Primitive::Sphere);
	GetMeshManager().LoadPrimitive(Primitive::Pyramid);

	GodCamera* godCam = GetGameObjectManager().CreateAndAdd<GodCamera>(*mainCamera);
	AddInputCallback(std::bind(&GodCamera::InputCallback, godCam, std::placeholders::_1));

	LoadContent();

	// TODO - Should have an engine level boolean, so as to not be dependent on the window's state
	while (window->IsActive())
	{
		EngineFrame();
	}

	delete world;
	GetTextureManager().Deinitialize();
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

void GameEngine::LoadContent()
{
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

void GameEngine::EngineFrame()
{
	UpdateAndRenderWorld();

	GatherFrameMetrics();
}

void GameEngine::UpdateAndRenderWorld()
{
	SCOPED_TIMED_BLOCK(UpdateAndRender);
	const float tick = .16f;
	InputUpdate();
	//GetInputManager().Update();

	BEGIN_TIMED_BLOCK(Update);
	world->TickWorld(tick);
	world->PushToRenderState();
	END_TIMED_BLOCK(Update);

	BEGIN_TIMED_BLOCK(Render);
	world->RenderWorld();
	END_TIMED_BLOCK(Render);
}

void GameEngine::GatherFrameMetrics()
{
	{
		//SCOPED_TIMED_BLOCK(GatherMetrics);
		GetProfilingStatistics().CollectAllFrameMetrics();
	}
	GetProfilingStatistics().ProfileFrameIncrement();
}

