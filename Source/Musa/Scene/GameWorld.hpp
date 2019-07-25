#pragma once

#include "Types/Intrinsics.hpp"
#include "Graphics/GraphicsResourceDefinitions.hpp"

class Scene;
class Window;

// This will contain the main scene that exists, as well as the physics
class GameWorld final
{
public:
	GameWorld();
	~GameWorld();

	void Initialize(const Window& window);

	// These are just pass throughs right now
	void TickWorld(float deltaTime);
	void PushToRenderState();
	void RenderWorld();

	Scene& GetScene();

private:
	Viewport viewport;
	Scene* scene;
};