#pragma once

#include "Containers/Array.h"
#include "PCSTree/PCSTree.h"

#include "GameObject/GameObject.h"

class Light;


// TODO - this structure would correspond to a file that records all of the objects in scene

// A Scene contains a hierarchy of game objects/entities.
// This will be the manager for those objects/entities
// The objects/entities will either be active or inactive
// There will be a corresponding Scene-like class that exists solely for holding rendering information
class Scene
{
public:
	void AddGameObjectToScene(GameObject& obj);
	void RemoveGameObjectFromScene(GameObject& obj);

	void AddLightToScene(Light& light);
	void RemoveLightFromScene(Light& light);

	void Tick(float deltaTime);
	
private:
	PCSTree<GameObject> sceneGraph;
	Array<GameObject*> activeGameObjects;
	Array<GameObject*> gameObjectsInScene;
	
	// Only allowing 3 lights per scene at the moment. This is because
	// the engine will become fully deferred, so there won't really be 
	// a hard limit on what lights affect what geometry
	Light* lights[3] = { nullptr, nullptr, nullptr };
};