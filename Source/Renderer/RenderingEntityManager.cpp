#include "RenderingEntityManager.h"
#include "RenderingEntity.h"

RenderingEntity * RenderingEntityManager::CreateOrGetEntity(Renderer* renderer, Model* model)
{
	EntityNode* current = Instance().head;
	RenderingEntity* entity = nullptr;
	while (current != nullptr)
	{
		if (current->renderingEntity->GetCPUModel() == model)
		{
			entity = current->renderingEntity;
			break;
		}
		current = current->next;
	}

	if (entity == nullptr)
	{
		EntityNode* node = new EntityNode;
		entity = new RenderingEntity(renderer, model);
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

void RenderingEntityManager::RemoveEntity(Model * model)
{
	UNUSED(model);
}

RenderingEntityManager & RenderingEntityManager::Instance()
{
	static RenderingEntityManager remInstance;
	return remInstance;
}

RenderingEntityManager::~RenderingEntityManager()
{
}
