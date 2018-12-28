#pragma once

#include "Graphics.h"
#include "Containers/Array.h"

#include "VulkanAbstractions.h"

class VulkanDevice;

class VulkanDescriptorSetLayout
{
public:
	VulkanDescriptorSetLayout(VulkanDevice* device);
	~VulkanDescriptorSetLayout();

	void AddDescriptorBinding(VkDescriptorType descriptorType, VkShaderStageFlags shaderStages, uint32 binding);
	void BindLayout();

	inline VkDescriptorSetLayout GetNativeHandle() const { return descriptorSetLayout; }

	inline Array<VkDescriptorSetLayoutBinding> GetLayoutBindings() const { return layoutBindings; }

private:
	VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
	Array<VkDescriptorSetLayoutBinding> layoutBindings;
	VulkanDevice* logicalDevice;
};

class VulkanDescriptorSet
{
public:
	VulkanDescriptorSet(VulkanDevice* device, VkDescriptorSet ds, VulkanDescriptorSetLayout* layout);
	~VulkanDescriptorSet();

	inline VkDescriptorSet GetNativeHandle() const { return descriptorSet; }

	void SetUniformBufferInfo(const VulkanBuffer& uniformBuffer, uint32 bufferBinding);
	void SetStorageBufferInfo(const VulkanBuffer& storageBuffer, uint32 bufferBinding);
	void SetSampledTextureInfo(const VulkanTexture& texture, uint32 textureBinding);

	void UpdateDescriptorSet();

private:
	VkDescriptorSet descriptorSet;
	VulkanDevice* logicalDevice;
	VulkanDescriptorSetLayout* associatedLayout;
	Array<VkDescriptorBufferInfo> bufferInfos;
	Array<VkDescriptorImageInfo> imageInfos;
	Array<VkWriteDescriptorSet> descriptorWrites;
	uint32 pad[1];
};