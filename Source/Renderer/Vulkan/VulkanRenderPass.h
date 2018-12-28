#pragma once

#include "Graphics.h"

#if GRAPHICS_API_VK

class VulkanDevice;

class VulkanRenderPass
{
public:
	VulkanRenderPass(VulkanDevice* device, VkFormat colorFormat);
	~VulkanRenderPass();

	inline VkRenderPass GetNativeHandle() const { return renderPass; }
	inline bool HasDepthStencil() const { return hasDepthStencil; }
	inline VkFormat GetDepthFormat() const { return depthFormat; }

private:
	VkRenderPass renderPass;
	VulkanDevice* logicalDevice;
	VkFormat depthFormat;
	bool hasDepthStencil;
	bool pad[7] = { false, false, false, false, false, false, false };

};

#endif