#pragma once

#include "EngineCore/Types.h"

class Scene;

// This will contain the main scene that exists, as well as the physics
class GameWorld final
{
public:
	GameWorld();
	~GameWorld();

	void TickWorld(float deltaTime);

	Scene& GetScene();

private:
	Scene* scene;
};