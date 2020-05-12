// Copyright 2020, Nathan Blane

#pragma once

#include "Types/Uncopyable.hpp"
#include "GameObject/GameObject.h"

class GameWorld;

class GameObjectManager final : private Uncopyable
{
public:
	GameObjectManager(GameWorld& world);

	//template<class GOType, typename = std::is_base_of_v<GameObject, GOType>>
	template<class GOType, typename... Args>
	GOType* CreateAndAdd(Args&&... args)
	{
		GOType* gameObj = new GOType(std::forward<Args>(args)...);
		Add(gameObj);
		return gameObj;
	}
	void Add(GameObject* go);
	void Remove(GameObject* go);

private:
	// TODO - Move to scene class
	GameWorld* world = nullptr;
};

