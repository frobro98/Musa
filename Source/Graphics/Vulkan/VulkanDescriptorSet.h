#pragma once

#include "Graphics.h"
#include "Containers/DynamicArray.hpp"

#include "VulkanAbstractions.h"

class VulkanDevice;
class VulkanFence;
class VulkanCommandBuffer;
class WriteDescriptorSet;
struct TextureSampler;

class VulkanDescriptorSetLayout
{
public:
	VulkanDescriptorSetLayout(VulkanDevice& device);
	~VulkanDescriptorSetLayout();

	VulkanDescriptorSetLayout& operator=(const VulkanDescriptorSetLayout&) = delete;

	void AddDescriptorBinding(VkDescriptorType descriptorType, VkShaderStageFlags shaderStages, uint32 binding);
	void BindLayout();

	inline VkDescriptorSetLayout GetNativeHandle() const { return descriptorSetLayout; }

	inline DynamicArray<VkDescriptorSetLayoutBinding> GetLayoutBindings() const { return layoutBindings; }

private:
	VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
	DynamicArray<VkDescriptorSetLayoutBinding> layoutBindings;
	VulkanDevice& logicalDevice;
};

class VulkanDescriptorSet
{
public:
	VulkanDescriptorSet(const VulkanDevice& device, VulkanFence* fence, VkDescriptorSet ds, VulkanDescriptorSetLayout* layout);

	VulkanDescriptorSet& operator=(const VulkanDescriptorSet&) = delete;

	inline VkDescriptorSet GetNativeHandle() const { return descriptorSet; }
	inline const VulkanFence* GetFence() const { return associatedFence; }

	void SetUniformBufferInfo(const VulkanBuffer& uniformBuffer, uint32 bufferBinding);
	void SetStorageBufferInfo(const VulkanBuffer& storageBuffer, uint32 bufferBinding);
	void SetSampledTextureInfo(const VulkanTexture& texture, const TextureSampler& sampler, uint32 textureBinding);

	void UpdateDescriptorSet();
	void UpdateDescriptorSet(const WriteDescriptorSet& writeDescriptors);

public:
	uint64 associatedFenceSignal;
	VulkanCommandBuffer* associatedCommandBuffer;

private:
	VkDescriptorSet descriptorSet;
	DynamicArray<VkDescriptorBufferInfo> bufferInfos;
	DynamicArray<VkDescriptorImageInfo> imageInfos;
	DynamicArray<VkWriteDescriptorSet> descriptorWrites;
	const VulkanDevice& logicalDevice;
	const VulkanDescriptorSetLayout* associatedLayout;
	const VulkanFence* associatedFence;
};
