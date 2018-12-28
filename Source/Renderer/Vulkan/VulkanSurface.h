#pragma once

#include "Graphics.h"
#include "Containers/Array.h"

class Window;
class VulkanDevice;
class VulkanSwapchain;

class VulkanSurface
{
public:
	VulkanSurface(VulkanDevice* device, const Window* windowSurface);
	~VulkanSurface();

	inline VkSurfaceKHR GetNativeHandle() const { return surfaceHandle; }

	Array<VkPresentModeKHR> GetPresentModes() const;
	Array<VkSurfaceFormatKHR> GetSurfaceFormats() const;
	VkSurfaceCapabilitiesKHR GetSurfaceCapabilities() const;

	int32 GetSurfaceWidth() const;
	int32 GetSurfaceHeight() const;

private:
	VkSurfaceCapabilitiesKHR surfaceCapabilities = {};
	// TODO - consider making this padding into a Vulkan struct
	uint32 pad[1];
	VkSurfaceKHR surfaceHandle = VK_NULL_HANDLE;
	Array<VkPresentModeKHR> presentModes;
	Array<VkSurfaceFormatKHR> surfaceFormats;
	VulkanDevice* logicalDevice;
	const Window* window;
};