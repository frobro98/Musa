#pragma once

#include "VulkanDefinitions.h"

#include "Containers/DynamicArray.hpp"

class VulkanDevice;
struct RenderTargetDescription;

class VulkanRenderPass
{
public:
	VulkanRenderPass(const VulkanDevice& device, const RenderTargetDescription& targets);
	~VulkanRenderPass();

	inline VkRenderPass GetNativeHandle() const { return renderPass; }
	inline DynamicArray<VkAttachmentDescription> GetAttachments() const { return attachments; }

private:
	VkRenderPass renderPass;
	DynamicArray<VkAttachmentDescription> attachments;
	const VulkanDevice* logicalDevice;
};
