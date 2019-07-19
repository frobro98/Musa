
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

#include "GameEngine.hpp"

void GameEngine::RunEngine()
{
	int32 width = 1080;
	int32 height = 720;

	Window* window = new Window(width, height);
	window->Initialize();
	window->SetAsActiveWindow();

	// TODO - This call needs some sort of type safety for the window handle...
	GetGraphicsInterface().InitializeGraphics(window->GetWindowHandle(), width, height);

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

	MeshManager::LoadPrimitive(Primitive::Box);
	MeshManager::LoadPrimitive(Primitive::Plane);
	MeshManager::LoadPrimitive(Primitive::Sphere);
	MeshManager::LoadPrimitive(Primitive::Pyramid);

	LoadContent();

	// TODO - Should have an engine level boolean, so as to not be dependent on the window's state
	while (window->IsActive())
	{
		EngineFrame();
	}

}

void GameEngine::LoadContent()
{
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


