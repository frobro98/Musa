#pragma once

class Model;
class ComputeEntity;

class ComputeEntityManager
{
public:
	static ComputeEntity* CreateOrGetEntity(Model& model);
	static void RemoveEntity(Model& model);

private:
	static ComputeEntityManager& Instance();

	ComputeEntityManager() = default;
	~ComputeEntityManager();
private:
	struct EntityNode
	{
		EntityNode* next = nullptr;
		ComputeEntity* computeEntity = nullptr;
	};

private:
	EntityNode* head = nullptr;
};