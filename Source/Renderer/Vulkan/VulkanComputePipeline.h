#pragma once

#include "Containers/Array.h"
#include "Graphics.h"

class VulkanDevice;
class VulkanCommandBuffer;
class VulkanDescriptorSet;
class VulkanPipelineLayout;
class VulkanDescriptorSetLayout;

class VulkanComputePipeline
{
public:
	VulkanComputePipeline(VulkanDevice* device);
	~VulkanComputePipeline();

	void Initialize(const VulkanPipelineLayout* layout, class VulkanShader* shader);

	void Bind(VulkanCommandBuffer* cmdBuffer) const;
	void BindDescriptorSet(VulkanCommandBuffer* cmdBuffer, VulkanDescriptorSet* descriptorSet) const;
	VulkanDescriptorSet* GetUnusedDescriptorSet();
	void ResetPipeline();
private:
	VkPipeline pipeline;
	const VulkanPipelineLayout* pipelineLayout;
	VulkanDevice* logicalDevice;
	Array<VulkanDescriptorSet*> descriptorSets;
	uint32 unusedSetIndex;
};
