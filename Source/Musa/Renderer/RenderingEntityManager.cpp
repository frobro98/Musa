#include "RenderingEntityManager.h"
#include "RenderingEntity.h"
#include "Shader/MaterialRenderInfo.hpp"
#include "Model/MeshSceneInfo.hpp"

RenderingEntity* RenderingEntityManager::CreateOrGetEntity(Renderer* renderer, MeshRenderInfo& meshInfo)
{
	if (meshInfo.meshMaterial->shadingModel == ShadingModel::Lit)
	{
		return CreateOrGetLit(renderer, meshInfo);
	}
	else
	{
		return CreateOrGetUnlit(renderer, meshInfo);
	}
}

void RenderingEntityManager::RemoveEntity(Model* model)
{
	UNUSED(model);
}

RenderingEntity* RenderingEntityManager::CreateOrGetUnlit(Renderer* renderer, MeshRenderInfo& meshInfo)
{
	EntityNode* current = Instance().unlitHead;
	RenderingEntity* entity = nullptr;
	while (current != nullptr)
	{
		if (current->renderingEntity->GetCPUInfo() == &meshInfo)
		{
			entity = current->renderingEntity;
			break;
		}
		current = current->next;
	}

	if (entity == nullptr)
	{
		EntityNode* node = new EntityNode;
		entity = new RenderingEntity(renderer, meshInfo);
		node->renderingEntity = entity;
		node->next = Instance().unlitHead;
		Instance().unlitHead = node;
	}
	else
	{
		entity->SetRenderer(renderer);
	}

	return entity;
}

RenderingEntity* RenderingEntityManager::CreateOrGetLit(Renderer* renderer, MeshRenderInfo& meshInfo)
{
	EntityNode* current = Instance().head;
	RenderingEntity* entity = nullptr;
	while (current != nullptr)
	{
		if (current->renderingEntity->GetCPUInfo() == &meshInfo)
		{
			entity = current->renderingEntity;
			break;
		}
		current = current->next;
	}

	if (entity == nullptr)
	{
		EntityNode* node = new EntityNode;
		entity = new RenderingEntity(renderer, meshInfo);
		node->renderingEntity = entity;
		node->next = Instance().head;
		Instance().head = node;
	}
	else
	{
		entity->SetRenderer(renderer);
	}

	return entity;
}

RenderingEntityManager& RenderingEntityManager::Instance()
{
	static RenderingEntityManager remInstance;
	return remInstance;
}

RenderingEntityManager::~RenderingEntityManager()
{
}
