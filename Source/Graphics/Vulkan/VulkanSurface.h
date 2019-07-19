#pragma once

#include "Graphics.h"
#include "Containers/DynamicArray.hpp"

class Window;
class VulkanDevice;
class VulkanSwapchain;

class VulkanSurface
{
public:
	VulkanSurface(VkInstance instance, VulkanDevice* device, void* windowHandle, uint32 width, uint32 height);
	~VulkanSurface();

	inline VkSurfaceKHR GetNativeHandle() const { return surfaceHandle; }

	DynamicArray<VkPresentModeKHR> GetPresentModes() const;
	DynamicArray<VkSurfaceFormatKHR> GetSurfaceFormats() const;
	VkSurfaceCapabilitiesKHR GetSurfaceCapabilities() const;

	int32 GetSurfaceWidth() const;
	int32 GetSurfaceHeight() const;

private:
	VkSurfaceCapabilitiesKHR surfaceCapabilities = {};
	// TODO - consider making this padding into a Vulkan struct
	VkInstance instance = VK_NULL_HANDLE;
	VkSurfaceKHR surfaceHandle = VK_NULL_HANDLE;
	DynamicArray<VkPresentModeKHR> presentModes;
	DynamicArray<VkSurfaceFormatKHR> surfaceFormats;
	VulkanDevice* logicalDevice;
	void* windowHandle;
	uint32 width;
	uint32 height;
};