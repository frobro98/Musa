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

void VulkanViewport::AcquireBackBuffer()
{
	// TODO - Remove [[maybe_unused]] from Musa because they honestly shouldn't be here unless specified
	[[maybe_unused]] VkResult result = swapchain->GetNextImage();
	Assert(result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR);
	backBufferTexture = &swapchain->GetSwapchainTarget();
	Assert(backBufferTexture);
}

void VulkanViewport::SubmitFrame()
{
	swapchain->SubmitFrame();
}

void VulkanViewport::PresentFrame()
{
	swapchain->Present();
}
