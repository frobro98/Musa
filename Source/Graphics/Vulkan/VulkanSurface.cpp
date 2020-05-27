// Copyright 2020, Nathan Blane

#include "VulkanSurface.h"
#include "VulkanDevice.h"
#include "VulkanQueue.h"
#include "VulkanCreateInfos.h"


VulkanSurface::VulkanSurface(VkInstance inst, VulkanDevice* device, void* handle, u32 w, u32 h) 
	: logicalDevice(device), windowHandle(handle),
	width(w), height(h)
{
	instance = inst;
	// TODO - REmove Windows specific calls and casts
	HINSTANCE hInstance = reinterpret_cast<HINSTANCE>(GetModuleHandle(nullptr));
	HWND hWnd = reinterpret_cast<HWND>(handle);
	VkWin32SurfaceCreateInfoKHR surfaceInfo = Vk::SurfaceInfo(hInstance, hWnd);

	VkPhysicalDevice gpu = logicalDevice->GetPhysicalDevice();

	VkResult result = vkCreateWin32SurfaceKHR(instance, &surfaceInfo, nullptr, &surfaceHandle);
	CHECK_VK(result);

	VkBool32 presentationSupported = VK_FALSE;
	vkGetPhysicalDeviceSurfaceSupportKHR(gpu, logicalDevice->GetGraphicsQueue()->GetFamilyIndex(), surfaceHandle, &presentationSupported);
	Assert(presentationSupported == VK_TRUE);

	// Get present modes
	u32 presentModeCount;
	result = vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surfaceHandle, &presentModeCount, nullptr);
	CHECK_VK(result);
	presentModes.Resize(presentModeCount);
	result = vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surfaceHandle, &presentModeCount, presentModes.GetData());
	CHECK_VK(result);

	// Get formats
	u32 formatCount = 0;
	result = vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surfaceHandle, &formatCount, nullptr);
	CHECK_VK(result);
 	surfaceFormats.Resize(formatCount);
	result = vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surfaceHandle, &formatCount, surfaceFormats.GetData());
 	CHECK_VK(result);

	result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surfaceHandle, &surfaceCapabilities);
	CHECK_VK(result);
}

VulkanSurface::~VulkanSurface()
{
	vkDestroySurfaceKHR(instance, surfaceHandle, nullptr);
}

DynamicArray<VkPresentModeKHR> VulkanSurface::GetPresentModes() const
{
	return presentModes;
}

DynamicArray<VkSurfaceFormatKHR> VulkanSurface::GetSurfaceFormats() const
{
	return surfaceFormats;
}

VkSurfaceCapabilitiesKHR VulkanSurface::GetSurfaceCapabilities() const
{
	return surfaceCapabilities;
}

i32 VulkanSurface::GetSurfaceWidth() const
{
	return width;
}

i32 VulkanSurface::GetSurfaceHeight() const
{
	return height;
}
