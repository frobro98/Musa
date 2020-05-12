// Copyright 2020, Nathan Blane

#pragma once

#include "Containers/DynamicArray.hpp"
#include "GameObject/GameObject.h"
#include "Graphics/RenderTargetDescription.hpp"

class Light;
class ScreenView;
class Viewport;
class RenderContext;
class RenderObjectManager;
struct MeshRenderInfo;
struct GBuffer;
struct SceneRenderTargets;
struct RenderTarget;

// TODO - this structure would correspond to a file that records all of the objects in scene

// A Scene contains a hierarchy of game objects/entities.
// This will be the manager for those objects/entities
// The objects/entities will either be active or inactive
// There will be a corresponding Scene-like class that exists solely for holding rendering information
class Scene
{
	static constexpr uint32 MaxLights = 3;
public:
	~Scene();

	void AddGameObjectToScene(GameObject& object);
	void RemoveGameObjectFromScene(GameObject& object);
	void AddMeshInfoToScene(MeshRenderInfo& obj);
	void RemoveMeshInfoFromScene(MeshRenderInfo& obj);

	void AddLightToScene(Light& light);
	void RemoveLightFromScene(Light& light);

	void Tick(float deltaTime);

	inline const FixedArray<Light*, MaxLights>& GetLights() const { return lights; }

private:

	//PCSTree<GameObject> sceneGraph;
	DynamicArray<GameObject*> activeGameObjects;
	DynamicArray<GameObject*> gameObjectsInScene;

	DynamicArray<MeshRenderInfo*> renderingInfo;
	
	// Only allowing 3 lights per scene at the moment. This is because
	// the engine will become fully deferred, so there won't really be 
	// a hard limit on what lights affect what geometry
	FixedArray<Light*, MaxLights> lights;
};