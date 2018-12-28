
#include "VulkanSurface.h"
#include "VulkanDevice.h"
#include "VulkanQueue.h"
#include "VulkanRenderer.h"
#include "Windowing/Window.h"
#include "VulkanCreateInfos.h"

#if GRAPHICS_API_VK

VulkanSurface::VulkanSurface(VulkanDevice* device, const Window* windowSurface) 
	: logicalDevice(device), window(windowSurface)
{
	VkInstance instance = logicalDevice->GetRenderContext()->GetNativeHandle();
	HINSTANCE hInstance = reinterpret_cast<HINSTANCE>(GetModuleHandle(nullptr));
	HWND hWnd = reinterpret_cast<HWND>(window->GetWindowHandle());
	VkWin32SurfaceCreateInfoKHR surfaceInfo = Vk::SurfaceInfo(hInstance, hWnd);

	CHECK_VK(vkCreateWin32SurfaceKHR(instance, &surfaceInfo, nullptr, &surfaceHandle));

	VkBool32 presentationSupported = VK_FALSE;
	vkGetPhysicalDeviceSurfaceSupportKHR(logicalDevice->GetPhysicalDevice(), logicalDevice->GetPresentationQueue()->GetFamilyIndex(), surfaceHandle, &presentationSupported);
	assert(presentationSupported == VK_TRUE);

	// Get present modes
	uint32 presentModeCount;
	CHECK_VK(vkGetPhysicalDeviceSurfacePresentModesKHR(logicalDevice->GetPhysicalDevice(), surfaceHandle, &presentModeCount, nullptr));
	presentModes.Resize(presentModeCount);
	CHECK_VK(vkGetPhysicalDeviceSurfacePresentModesKHR(logicalDevice->GetPhysicalDevice(), surfaceHandle, &presentModeCount, presentModes.GetData()));

	// Get formats
	uint32 formatCount = 0;
	CHECK_VK(vkGetPhysicalDeviceSurfaceFormatsKHR(logicalDevice->GetPhysicalDevice(), surfaceHandle, &formatCount, nullptr));
 	surfaceFormats.Resize(formatCount);
 	CHECK_VK(vkGetPhysicalDeviceSurfaceFormatsKHR(logicalDevice->GetPhysicalDevice(), surfaceHandle, &formatCount, surfaceFormats.GetData()));

	CHECK_VK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(logicalDevice->GetPhysicalDevice(), surfaceHandle, &surfaceCapabilities));
}

VulkanSurface::~VulkanSurface()
{
	vkDestroySurfaceKHR(logicalDevice->GetRenderContext()->GetNativeHandle(), surfaceHandle, nullptr);
}

Array<VkPresentModeKHR> VulkanSurface::GetPresentModes() const
{
	return presentModes;
}

Array<VkSurfaceFormatKHR> VulkanSurface::GetSurfaceFormats() const
{
	return surfaceFormats;
}

VkSurfaceCapabilitiesKHR VulkanSurface::GetSurfaceCapabilities() const
{
	return surfaceCapabilities;
}

int32 VulkanSurface::GetSurfaceWidth() const
{
	return window->GetWidth();
}

int32 VulkanSurface::GetSurfaceHeight() const
{
	return window->GetHeight();
}

#endif