#pragma once

// TODO - Rename the "Types" folder to be "BasicTypes" which shows that the types in it are sort of intrinsic to Musa
#include "Types/Intrinsics.hpp"
#include "Types/UniquePtr.hpp"
#include "Scene/ScreenView.hpp"
#include "GameObject/GameObjectManager.h"
#include "GameObject/RenderObjectManager.hpp"
#include "Scene/Viewport.hpp"

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
	~GameWorld();

	// These are just pass throughs right now
	void TickWorld(float deltaTime);
	void PushToRenderState();
	void RenderWorld(const GBuffer& gbuffer, const SceneRenderTargets& sceneTargets, RenderTarget& uiTarget, Viewport& viewport);


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

private:
	UniquePtr<GameObjectManager> gameObjectManager;
	UniquePtr<RenderObjectManager> renderObjectManager;
	UniquePtr<ScreenView> screenView;
	Scene* scene;
};