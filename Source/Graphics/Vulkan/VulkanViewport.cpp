#include "VulkanViewport.hpp"
#include "VulkanSurface.h"
#include "VulkanSwapchain.h"

VulkanViewport::VulkanViewport(VulkanDevice& device, VkInstance instance, void* windowHandle, uint32 viewWidth, uint32 viewHeight)
	: logicalDevice(device)
{
	viewSurface = new VulkanSurface(instance, &logicalDevice, windowHandle, viewWidth, viewHeight);
	swapchain = new VulkanSwapchain(logicalDevice, viewSurface);
	swapchain->Initialize();
}

VulkanViewport::~VulkanViewport()
{
	delete swapchain;
	delete viewSurface;
}

bool VulkanViewport::CanProceedWithRender()
{
	VkResult result = swapchain->GetNextImage();
	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		swapchain->Recreate();
		return false;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		// TODO - log
		Assert(false);
		return false;
	}

	return true;
}

void VulkanViewport::SubmitFrame()
{
	swapchain->SubmitFrame();
	swapchain->Present();
}
