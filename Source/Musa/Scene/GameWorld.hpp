#pragma once

#include "EngineCore/Types/Intrinsics.hpp"

class Scene;
class Window;

// This will contain the main scene that exists, as well as the physics
class GameWorld final
{
public:
	GameWorld();
	~GameWorld();

	void Initialize(const Window& window);

	// These are just passthroughs right now
	void TickWorld(float deltaTime);
	void PushToRenderState();
	void RenderWorld();

	Scene& GetScene();

private:
	Scene* scene;

	// TODO - Make this something that doesn't live anywhere 
	class Renderer* renderEngine;
};