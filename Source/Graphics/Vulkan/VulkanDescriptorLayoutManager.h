#pragma once

#include "Containers/DynamicArray.hpp"

class VulkanDescriptorSetLayout;
class VulkanDevice;


// TODO - This is a temporary class that should be removed later on. There needs to be a better system, will probably be managed by whatever creates pipelines
class VulkanDescriptorLayoutManager
{
public:
	VulkanDescriptorLayoutManager() = default;
	~VulkanDescriptorLayoutManager();

	VulkanDescriptorSetLayout* CreateSetLayout();

	static VulkanDescriptorLayoutManager& Instance();

private:
	friend class VulkanDevice;
	DynamicArray<VulkanDescriptorSetLayout*> layouts;
	VulkanDevice* logicalDevice;
};

VulkanDescriptorLayoutManager& GetDescriptorLayoutManager();
