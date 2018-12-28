
#include "VulkanDescriptorPool.h"
#include "VulkanDevice.h"
#include "VulkanDescriptorSet.h"

VulkanDescriptorPool::VulkanDescriptorPool(VulkanDevice * device)
	: logicalDevice(device)
{
	assert(logicalDevice != nullptr);

	VkPhysicalDeviceLimits limits = logicalDevice->GetDeviceLimits();
	maxSamplerPoolSize = limits.maxDescriptorSetSamplers;
	maxUniformBufferPoolSize = limits.maxDescriptorSetUniformBuffers;
	maxDynamicUniformBufferPoolSize = limits.maxDescriptorSetUniformBuffersDynamic;
	maxStorageBufferPoolSize = limits.maxDescriptorSetStorageBuffers;
	maxDynamicStorageBufferPoolSize = limits.maxDescriptorSetStorageBuffersDynamic;
	maxStorageImagePoolSize = limits.maxDescriptorSetStorageImages;
	maxSampledImagePoolSize = limits.maxDescriptorSetSampledImages;
	maxInputAttachmentPoolSize = limits.maxDescriptorSetInputAttachments;

	// TODO - Initialization was failing. Need to figure out why the limits weren't working
	VkDescriptorPoolSize poolSizes[8] = {};
	poolSizes[0].descriptorCount = 10000;//maxSamplerPoolSize;
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = 10000;//maxUniformBufferPoolSize;
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[2].descriptorCount = maxDynamicUniformBufferPoolSize;
	poolSizes[2].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	poolSizes[3].descriptorCount = 10000;//maxStorageBufferPoolSize;
	poolSizes[3].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	poolSizes[4].descriptorCount = maxDynamicStorageBufferPoolSize;
	poolSizes[4].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
	poolSizes[5].descriptorCount = 10000;// maxStorageImagePoolSize;
	poolSizes[5].type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
	poolSizes[6].descriptorCount = 10000;// maxSampledImagePoolSize;
	poolSizes[6].type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
	poolSizes[7].descriptorCount = 10000;// maxInputAttachmentPoolSize;
	poolSizes[7].type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = ArraySize(poolSizes);
	poolInfo.pPoolSizes = poolSizes;
	poolInfo.maxSets = 1000;//logicalDevice->GetDeviceLimits().maxBoundDescriptorSets;
	// TODO - Figure out what this flag specifically does
	poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	CHECK_VK(vkCreateDescriptorPool(logicalDevice->GetNativeHandle(), &poolInfo, nullptr, &descriptorPool));
}

VulkanDescriptorPool::~VulkanDescriptorPool()
{
	vkDestroyDescriptorPool(logicalDevice->GetNativeHandle(), descriptorPool, nullptr);
}

VulkanDescriptorSet * VulkanDescriptorPool::AllocateDescriptorSet(VulkanDescriptorSetLayout* setLayout)
{
	// TODO - track descriptors
	VkDescriptorSetLayout layouts[] = { setLayout->GetNativeHandle() };
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = layouts;

	VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
	CHECK_VK(vkAllocateDescriptorSets(logicalDevice->GetNativeHandle(), &allocInfo, &descriptorSet));

	return new VulkanDescriptorSet(logicalDevice, descriptorSet, setLayout);
}