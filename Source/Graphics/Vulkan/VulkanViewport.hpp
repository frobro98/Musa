#pragma once

#include "Graphics.h"

class VulkanSwapchain;
class VulkanSurface;
class VulkanDevice;

// TODO - Figure out how to represent native resources. 
// Currently, there can should either be a table that has handles to resources or derive from set resources essentially for type safety

// Represents the viewport of the entire window
class VulkanViewport final
{
public:
	VulkanViewport(VulkanDevice& device, VkInstance instance, void* windowHandle, uint32 viewWidth, uint32 viewHeight);
	~VulkanViewport();

	bool CanProceedWithRender();
	void SubmitFrame();

	inline uint32 GetWidth() const { return viewportWidth; }
	inline uint32 GetHeight() const { return viewportHeight; }
	inline VulkanSwapchain& GetSwapchain() const { return *swapchain; }

private:
	VulkanDevice& logicalDevice;
	VulkanSurface* viewSurface;
	VulkanSwapchain* swapchain;
	uint32 viewportWidth, viewportHeight;
};