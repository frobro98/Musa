#pragma once

#include "Containers/Array.h"
#include "VulkanRenderingInfo.h"

class VulkanDevice;
class VulkanRenderPass;
class VulkanDescriptorSet;
class VulkanCommandBuffer;
class VulkanDescriptorSetLayout;

class VulkanPipelineLayout
{
public:
	VulkanPipelineLayout(VulkanDevice* device, Array<VulkanDescriptorSetLayout*>&& layouts);
	~VulkanPipelineLayout();

	inline Array<VulkanDescriptorSetLayout*> GetDescriptorSetLayouts() const { return descriptorSetLayouts; }
	inline VkPipelineLayout GetNativeHandle() const { return pipelineLayout; }
private:
	VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
	Array<VulkanDescriptorSetLayout*> descriptorSetLayouts;
	VulkanDevice* logicalDevice;
};

class VulkanPipeline
{
public:
	VulkanPipeline(VulkanDevice* device);
	~VulkanPipeline();

	void Initialize(const VulkanPipelineLayout* layout, const RenderPipelineInfo& renderingInfo, VulkanRenderPass* renderPass);

	void Bind(VulkanCommandBuffer* cmdBuffer) const;
	void BindDescriptorSet(VulkanCommandBuffer* cmdBuffer, VulkanDescriptorSet* descriptorSet) const;
	VulkanDescriptorSet* GetUnusedDescriptorSet();
	void ResetPipeline();

private:
	VkPipeline pipeline;
	RenderPipelineInfo renderInfo;
	const VulkanPipelineLayout* pipelineLayout;
	VulkanDevice* logicalDevice;
	Array<VulkanDescriptorSet*> descriptorSets;
	uint32 unusedSetIndex = 0;
};
