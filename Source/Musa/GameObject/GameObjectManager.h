#pragma once

#include "GameObject/GameObject.h"

class GameWorld;

class GameObjectManager final
{
public:
	void Initialize(GameWorld& world);

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

	static GameObjectManager& Instance();

private:
	GameObjectManager();

	GameObjectManager(const GameObjectManager&) = delete;
	GameObjectManager& operator=(const GameObjectManager&) = delete;

	// TODO - Move to scene class
	GameWorld* world = nullptr;
};

GameObjectManager& GetGameObjectManager();
