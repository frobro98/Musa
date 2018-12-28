#include "GameObjectManager.h"
#include "Scene/SceneRoot.h"
#include "PCSTree/PCSTree.h"

#include "Scene/GameWorld.hpp"
#include "Scene/Scene.hpp"


void GameObjectManager::Initialize(GameWorld& world_)
{
	world = &world_;
}

void GameObjectManager::Add(GameObject* go)
{
	world->GetScene().AddGameObjectToScene(*go);
}

void GameObjectManager::Remove(GameObject* go)
{
	world->GetScene().RemoveGameObjectFromScene(*go);
	delete go;
}

GameObjectManager::GameObjectManager()
{
	GameObject* root = new SceneRoot;
	root->name = "Scene Root";
}

GameObjectManager& GameObjectManager::Instance()
{
	static GameObjectManager gomInstance;
	return gomInstance;
}

GameObjectManager& GetGameObjectManager()
{
	return GameObjectManager::Instance();
}
