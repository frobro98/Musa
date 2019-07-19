
#include "VulkanSurface.h"
#include "VulkanDevice.h"
#include "VulkanQueue.h"
#include "VulkanCreateInfos.h"


VulkanSurface::VulkanSurface(VkInstance inst, VulkanDevice* device, void* handle, uint32 w, uint32 h) 
	: logicalDevice(device), windowHandle(handle),
	width(w), height(h)
{
	instance = inst;
	HINSTANCE hInstance = reinterpret_cast<HINSTANCE>(GetModuleHandle(nullptr));
	HWND hWnd = reinterpret_cast<HWND>(handle);
	VkWin32SurfaceCreateInfoKHR surfaceInfo = Vk::SurfaceInfo(hInstance, hWnd);

	VkPhysicalDevice gpu = logicalDevice->GetPhysicalDevice();

	CHECK_VK(vkCreateWin32SurfaceKHR(instance, &surfaceInfo, nullptr, &surfaceHandle));

	VkBool32 presentationSupported = VK_FALSE;
	vkGetPhysicalDeviceSurfaceSupportKHR(gpu, logicalDevice->GetGraphicsQueue()->GetFamilyIndex(), surfaceHandle, &presentationSupported);
	Assert(presentationSupported == VK_TRUE);

	// Get present modes
	uint32 presentModeCount;
	CHECK_VK(vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surfaceHandle, &presentModeCount, nullptr));
	presentModes.Resize(presentModeCount);
	CHECK_VK(vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surfaceHandle, &presentModeCount, presentModes.GetData()));

	// Get formats
	uint32 formatCount = 0;
	CHECK_VK(vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surfaceHandle, &formatCount, nullptr));
 	surfaceFormats.Resize(formatCount);
 	CHECK_VK(vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surfaceHandle, &formatCount, surfaceFormats.GetData()));

	CHECK_VK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surfaceHandle, &surfaceCapabilities));
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

int32 VulkanSurface::GetSurfaceWidth() const
{
	return width;
}

int32 VulkanSurface::GetSurfaceHeight() const
{
	return height;
}
