#pragma once

#include "Graphics.h"

class VulkanDevice;
class VulkanDescriptorSet;
class VulkanDescriptorSetLayout;

class VulkanDescriptorPool
{
public:
	VulkanDescriptorPool(VulkanDevice& device);
	~VulkanDescriptorPool();

	VulkanDescriptorSet* AllocateDescriptorSet(VulkanDescriptorSetLayout* setLayout);


private:
	VkDescriptorPool descriptorPool;
	VulkanDevice& logicalDevice;

	uint32 maxSamplerPoolSize = 0;
	uint32 maxUniformBufferPoolSize = 0;
	uint32 maxStorageBufferPoolSize = 0;
	uint32 maxDynamicStorageBufferPoolSize = 0;
	uint32 maxDynamicUniformBufferPoolSize = 0;
	uint32 maxSampledImagePoolSize = 0;
	uint32 maxStorageImagePoolSize = 0;
	uint32 maxInputAttachmentPoolSize = 0;

	uint32 currentSamplerPool = 0;
	uint32 currentUniformBufferPool = 0;
	uint32 currentStorageBufferPool = 0;
	uint32 currentDynamicStorageBufferPool = 0;
	uint32 currentDynamicUniformBufferPool = 0;
	uint32 currentSampledImagePool = 0;
	uint32 currentStorageImagePool = 0;
	uint32 currentInputAttachmentPool = 0;
};