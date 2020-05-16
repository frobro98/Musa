// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "BasicTypes/UniquePtr.hpp"
#include "Scene/ScreenView.hpp"
#include "GameObject/GameObjectManager.h"
#include "GameObject/RenderObjectManager.hpp"
#include "Scene/Viewport.hpp"
#include "Scene/Scene.hpp"

class Scene;
class Window;
struct MeshRenderInfo;
struct GBuffer;
struct SceneRenderTargets;
struct RenderTarget;

// This will contain the main scene that exists, as well as the physics
class GameWorld final
{
public:
	GameWorld(const Window& window);

	// These are just pass throughs right now
	void TickWorld(float deltaTime);
	void PushToRenderState();


	// TODO - These functions are sort of hacky because I don't have a high level way of creating game objects yet. Working on it...
	template <class ObjectType, typename... Args>
	ObjectType* CreateGameObject(Args&&... args)
	{
		return gameObjectManager->CreateAndAdd<ObjectType>(*this, args...);
	}

	void RegisterRenderInfo(const GameObject& go, MeshRenderInfo& renderInfo);
	void UnregisterRenderInfo(const GameObject& go);

	// TODO - End GameObject creation hacks

	Scene& GetScene();

	// TODO - These getters are hacks and shouldn't be here. Rethink design!!!
	ScreenView& GetView() const { return *screenView; }
	RenderObjectManager& GetRenderManager() const { return *renderObjectManager; }

private:
	UniquePtr<GameObjectManager> gameObjectManager;
	UniquePtr<RenderObjectManager> renderObjectManager;
	UniquePtr<ScreenView> screenView;
	UniquePtr<Scene> scene;
};