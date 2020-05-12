// Copyright 2020, Nathan Blane

#pragma once

#include "Containers/DynamicArray.hpp"
#include "VulkanDefinitions.h"

class VulkanDevice;
class VulkanCommandBuffer;
class VulkanDescriptorSet;
class VulkanPipelineLayout;
class VulkanDescriptorSetLayout;

class VulkanComputePipeline
{
public:
	VulkanComputePipeline(VulkanDevice& device);
	~VulkanComputePipeline();

	void Initialize(const VulkanPipelineLayout* layout, class VulkanShader* shader);

	void Bind(VulkanCommandBuffer* cmdBuffer) const;
	void BindDescriptorSet(VulkanCommandBuffer* cmdBuffer, VulkanDescriptorSet* descriptorSet) const;
	VulkanDescriptorSet* GetUnusedDescriptorSet();
	void ResetPipeline();
private:
	VkPipeline pipeline;
	const VulkanPipelineLayout* pipelineLayout = nullptr;
	VulkanDevice& logicalDevice;
	DynamicArray<VulkanDescriptorSet*> descriptorSets;
	uint32 unusedSetIndex;
};
