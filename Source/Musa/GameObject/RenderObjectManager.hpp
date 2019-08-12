#pragma once

#include "Containers/Map.h"

class GameObject;
struct RenderObject;
struct MeshRenderInfo;
struct View;

class RenderObjectManager
{
public:
	void RegisterGameObject(const GameObject& obj, MeshRenderInfo& meshInfo);
	void UnregisterGameObject(const GameObject& obj);

	void SequenciallyPull();

	inline const DynamicArray<RenderObject*>& GetRenderObjects() const { return renderObjects; }

private:
	Map<const GameObject*, RenderObject*> gameobjectRegistry;
	DynamicArray<RenderObject*> renderObjects;
};

RenderObjectManager& GetRenderObjectManager();
