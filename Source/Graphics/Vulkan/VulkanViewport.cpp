// Copyright 2020, Nathan Blane

#include "VulkanDefinitions.h"
#include "VulkanViewport.hpp"
#include "VulkanSurface.h"
#include "VulkanSwapchain.h"

VulkanViewport::VulkanViewport(VulkanDevice& device, VkInstance instance, void* windowHandle, u32 viewWidth, u32 viewHeight)
	: logicalDevice(device)
{
	REF_CHECK(device);

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
	NOT_USED VkResult result = swapchain->GetNextImage();
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
