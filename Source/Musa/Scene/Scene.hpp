#pragma once

#include "Containers/DynamicArray.hpp"
//#include "PCSTree/PCSTree.h"
#include "GameObject/GameObject.h"
#include "Graphics/RenderTargetDescription.hpp"

class Light;
class ScreenView;
class Viewport;
class Renderer;
struct MeshRenderInfo;

class SceneRendering;

// TODO - this structure would correspond to a file that records all of the objects in scene

// A Scene contains a hierarchy of game objects/entities.
// This will be the manager for those objects/entities
// The objects/entities will either be active or inactive
// There will be a corresponding Scene-like class that exists solely for holding rendering information
class Scene
{
public:
	~Scene();

	void InitializeScene();

	void AddGameObjectToScene(GameObject& object);
	void RemoveGameObjectFromScene(GameObject& object);
	void AddMeshInfoToScene(MeshRenderInfo& obj);
	void RemoveMeshInfoFromScene(MeshRenderInfo& obj);

	void AddLightToScene(Light& light);
	void RemoveLightFromScene(Light& light);

	void Tick(float deltaTime);
	void PushStateToGpu();
	void RenderScene(Viewport& viewport);
	
	void SetView(ScreenView& view);

	inline RenderTargetDescription GetGBufferDescription() const { return gbufferTargets; }
	RenderTargetTextures& GetGBufferTargets() { return gbufferTextures; }
	inline const DynamicArray<MeshRenderInfo*> GetRenderInfo() const { return renderingInfo; }
	inline const ScreenView& GetScreenView() const { return *view; }
	inline Light** GetLights() { return lights; }
	
private:
	void SetupRenderView();
	void CreateGBuffer();

private:
	// TODO - GBuffer per scene doesn't really make any sense, but because there's only a single scene, it's aight for now
	RenderTargetDescription gbufferTargets{};
	RenderTargetTextures gbufferTextures{};

	//PCSTree<GameObject> sceneGraph;
	DynamicArray<GameObject*> activeGameObjects;
	DynamicArray<GameObject*> gameObjectsInScene;

	DynamicArray<MeshRenderInfo*> renderingInfo;
	
	// Only allowing 3 lights per scene at the moment. This is because
	// the engine will become fully deferred, so there won't really be 
	// a hard limit on what lights affect what geometry
	Light* lights[3] = { nullptr, nullptr, nullptr };

	Renderer* renderer;

	ScreenView* view = nullptr;
	SceneRendering* sceneRendering = nullptr;

	bool gbuffersInitialized = false;
};