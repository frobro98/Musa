#include "RenderObjectManager.hpp"
#include "RenderObject.hpp"


void RenderObjectManager::RegisterGameObject(const GameObject& obj, MeshRenderInfo& meshInfo)
{
	RenderObject* ro;
	if (!gameobjectRegistry.TryFind(&obj, ro))
	{
		ro = new RenderObject;
		ro->SetGameObject(obj, meshInfo);
		renderObjects.Add(ro);
	}
}

void RenderObjectManager::UnregisterGameObject(const GameObject& obj)
{
	RenderObject* ro;
	if (gameobjectRegistry.TryFind(&obj, ro))
	{
		ro->ResetGameObject();
		gameobjectRegistry.Remove(&obj);
		renderObjects.RemoveAll(ro);
		delete ro;
	}
}

void RenderObjectManager::SequenciallyPull(const View& view)
{
	for (auto& renderObject : renderObjects)
	{
		renderObject->PullDataFromGameObject(view);
	}
}

RenderObjectManager& GetRenderObjectManager()
{
	static RenderObjectManager roInstance;
	return roInstance;
}
