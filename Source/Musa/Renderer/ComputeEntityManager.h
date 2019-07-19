#pragma once

class Model;
class ComputeEntity;

// TODO - Remove vulkan references!!
class VulkanDevice;

class ComputeEntityManager
{
public:
	static ComputeEntity* CreateOrGetEntity(const VulkanDevice& device, Model& model);
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