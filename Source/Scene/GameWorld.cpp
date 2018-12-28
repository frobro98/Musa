
#include "GameWorld.hpp"
#include "Scene/Scene.hpp"

GameWorld::GameWorld()
	: scene(new Scene)
{
}

GameWorld::~GameWorld()
{
	delete scene;
}

void GameWorld::TickWorld(float deltaTime)
{
	scene->Tick(deltaTime);
}

Scene& GameWorld::GetScene()
{
	return *scene;
}
