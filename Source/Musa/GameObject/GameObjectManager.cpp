// Copyright 2020, Nathan Blane

#include "GameObjectManager.h"

#include "Scene/GameWorld.hpp"
#include "Scene/Scene.hpp"


GameObjectManager::GameObjectManager(GameWorld& world_)
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

