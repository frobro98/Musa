#include "Scene.hpp"
#include "Platform.h"
#include "GameObject/GameObject.h"

void Scene::AddGameObjectToScene(GameObject& obj)
{
	activeGameObjects.Add(&obj);
	obj.AssociateScene(*this);
}

void Scene::RemoveGameObjectFromScene(GameObject& obj)
{
	activeGameObjects.RemoveAll(&obj);
}

void Scene::AddLightToScene(Light& light)
{
	bool emptySlotFound = false;
	for (uint32 i = 0; i < ArraySize(lights); ++i)
	{
		if (lights[i] == nullptr)
		{
			lights[i] = &light;
			emptySlotFound = true;
			break;
		}
	}

	Assert(emptySlotFound);
}

void Scene::RemoveLightFromScene(Light& light)
{
	bool lightFound = false;
	for (uint32 i = 0; i < ArraySize(lights); ++i)
	{
		if (lights[i] == &light)
		{
			lights[i] = nullptr;
			lightFound = true;
			break;
		}
	}

	Assert(lightFound);
}

void Scene::Tick(float deltaTime)
{
// 	PCSTree<GameObject>::ForwardIterator iter(sceneGraph.GetRoot());
// 	while (!iter.IsDone())
// 	{
// 		GameObject* go = *iter;
// 		go->Update(deltaTime);
// 		++iter;
// 	}

	for (auto& go : activeGameObjects)
	{
		go->Update(deltaTime);
	}
}
