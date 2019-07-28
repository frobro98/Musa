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
	VulkanDescriptorSetLayout* layout = new VulkanDescriptorSetLayout(*logicalDevice);
	layouts.Add(layout);
	return layout;
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
