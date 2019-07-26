#pragma once

#include "Types/Intrinsics.hpp"
#include "Scene/Viewport.hpp"

class Scene;
class Window;

// This will contain the main scene that exists, as well as the physics
class GameWorld final
{
public:
	GameWorld(const Window& window);
	~GameWorld();

	// These are just pass throughs right now
	void TickWorld(float deltaTime);
	void PushToRenderState();
	void RenderWorld();

	Scene& GetScene();

private:
	Viewport viewport;
	Scene* scene;
};