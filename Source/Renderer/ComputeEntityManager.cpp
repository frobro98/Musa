#include "ComputeEntityManager.h"
#include "ComputeEntity.h"

ComputeEntity * ComputeEntityManager::CreateOrGetEntity(Model& model)
{
	EntityNode* current = Instance().head;
	ComputeEntity* entity = nullptr;
	while (current != nullptr)
	{
		if (current->computeEntity->GetCPUModel() == &model)
		{
			entity = current->computeEntity;
			break;
		}
		current = current->next;
	}

	if (entity == nullptr)
	{
		EntityNode* node = new EntityNode;
		entity = new ComputeEntity(model);
		node->computeEntity = entity;
		node->next = Instance().head;
		Instance().head = node;
	}

	return entity;
}

void ComputeEntityManager::RemoveEntity(Model& model)
{
	// TODO - Actually free memory correctly
	UNUSED(model);
}

ComputeEntityManager& ComputeEntityManager::Instance()
{
	static ComputeEntityManager cmInstance;
	return cmInstance;
}

ComputeEntityManager::~ComputeEntityManager()
{
}
