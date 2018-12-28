#pragma once

#include "Graphics.h"
#include "Containers/Array.h"

class VulkanDevice;
class VulkanRenderPass;

class VulkanFramebuffer
{
public:
	VulkanFramebuffer(VulkanDevice* device);
	~VulkanFramebuffer();
	VulkanFramebuffer(const VulkanFramebuffer&) = delete;
	VulkanFramebuffer& operator=(const VulkanFramebuffer&) = delete;

	void Initialize(const Array<VkImageView>& attachments, const VkExtent2D& extent, VulkanRenderPass* renderPass_);

	inline VkFramebuffer GetNativeHandle() const { return frameBuffer; }
	inline VulkanRenderPass* GetRenderPass() const { return renderPass; }
	inline VkExtent2D GetExtent() const { return framebufferExtent; }

private:
	VkFramebuffer frameBuffer = VK_NULL_HANDLE;
	VulkanDevice* logicalDevice;
	VulkanRenderPass* renderPass;
	Array<VkImageView> attachments;
	VkExtent2D framebufferExtent{ 0, 0 };
	uint32 pad[1];
};