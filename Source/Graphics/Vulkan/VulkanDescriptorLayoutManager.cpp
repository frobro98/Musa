#include "VulkanDescriptorLayoutManager.h"
#include "VulkanDescriptorSet.h"

VulkanDescriptorLayoutManager::~VulkanDescriptorLayoutManager()
{
	for (uint32 i = 0; i < layouts.Size(); ++i)
	{
		delete layouts[i];
	}
}

VulkanDescriptorSetLayout* VulkanDescriptorLayoutManager::CreateSetLayout()
{
	return new VulkanDescriptorSetLayout(*logicalDevice);
}

VulkanDescriptorLayoutManager& VulkanDescriptorLayoutManager::Instance()
{
	static VulkanDescriptorLayoutManager manager;
	return manager;
}

VulkanDescriptorLayoutManager& GetDescriptorLayoutManager()
{
	return VulkanDescriptorLayoutManager::Instance();
}
