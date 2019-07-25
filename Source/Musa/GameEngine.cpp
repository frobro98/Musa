
#include "Graphics/GraphicsInterface.hpp"
#include "Camera/Camera.h"
#include "Camera/CameraManager.h"
#include "Math/Rect.hpp"
#include "Types/Intrinsics.hpp"
#include "Math/Vector.h"
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

#include "Shader/ShaderObjects/ShaderDefinition.hpp"
#include "Shader/ShaderObjects/UnlitShading.hpp"
#include "Shader/ShaderObjects/BlinnShading.hpp"

#include "GameEngine.hpp"

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

	world = new GameWorld;
	world->Initialize(*window);

	GetGameObjectManager().Initialize(*world);

	GetInputManager().Initialize(*window);

	const float32 aspect = (float32)width / (float32)height;
	const IntRect viewport = { 0, 0, width, height };

	Camera* mainCamera = new Camera;
	mainCamera->SetViewport(viewport);
	mainCamera->SetPerspective(45.f, aspect, .1f, 10000.f);
	mainCamera->SetOrientationAndPosition(Vector(0, 0, 0), Vector(0, 0, 155.f), Vector(0, 1, 0));

	GetCameraManager().AddCamera(mainCamera, "Main Camera");
	GetCameraManager().SetActiveCamera("Main Camera");

	GetTextureManager().AddTexture(*::Internal::WhiteTexture);
	GetTextureManager().AddTexture(*::Internal::BlackTexture);

	GetMeshManager().LoadPrimitive(Primitive::Box);
	GetMeshManager().LoadPrimitive(Primitive::Plane);
	GetMeshManager().LoadPrimitive(Primitive::Sphere);
	GetMeshManager().LoadPrimitive(Primitive::Pyramid);

	GetGameObjectManager().CreateAndAdd<GodCamera>(*mainCamera);

	LoadContent();

	// TODO - Should have an engine level boolean, so as to not be dependent on the window's state
	while (window->IsActive())
	{
		EngineFrame();
	}

}

void GameEngine::LoadContent()
{
	Mesh* sphere = GetMeshManager().FindMesh(Mesh::SphereName);

	ShaderResource& vertShader = GetShader<BlinnVert>()->GetNativeShader();
	ShaderResource& fragShader = GetShader<BlinnFrag>()->GetNativeShader();

	GameObject* go = GetGameObjectManager().CreateAndAdd<GameObject>();
	go->SetModel(ModelFactory::CreateModel(sphere, new Material(vertShader, fragShader, ::Internal::WhiteTexture, Color32::Cyan())));
	go->SetScale(30, 30, 30);

	// TODO - A scene currently requires a light in it. This is very bad. It needs to be so lighting is done in a separate pass and then the screen shader renders to the screen...
	Light* light = GetGameObjectManager().CreateAndAdd<SpotLight>();
	light->SetPos(Vector(100, 100, 100));
	light->SetRotation(-40.f, 45.f, 0.f);
	world->GetScene().AddLightToScene(*light);
}

void GameEngine::EngineFrame()
{
	const float tick = .16f;
	GetInputManager().Update();

	world->TickWorld(tick);
	world->PushToRenderState();
	world->RenderWorld();
}


