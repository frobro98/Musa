
#include "VulkanSwapchain.h"
#include "VulkanDevice.h"
#include "VulkanFramebuffer.h"
#include "VulkanSurface.h"
#include "VulkanCreateInfos.h"
#include "VulkanRenderPass.h"
#include "VulkanUtilities.h"
#include "VulkanCommandBuffer.h"
#include "VulkanQueue.h"
#include "VulkanRenderingCloset.hpp"
#include "VulkanMemoryManager.hpp"

VulkanSwapchain::VulkanSwapchain(VulkanDevice& device, VulkanSurface* renderSurface)
	:logicalDevice(device), surface(renderSurface)
{
}

VulkanSwapchain::~VulkanSwapchain()
{
	for (auto tex : swapchainImageTargets)
	{
		vkDestroyImageView(logicalDevice.GetNativeHandle(), tex->imageView, nullptr);
	}
	vkDestroySemaphore(logicalDevice.GetNativeHandle(), imageAvailable, nullptr);
	vkDestroySemaphore(logicalDevice.GetNativeHandle(), renderingFinished, nullptr);
	Assert(swapchainHandle != VK_NULL_HANDLE);
	vkDestroySwapchainKHR(logicalDevice.GetNativeHandle(), swapchainHandle, nullptr);
}

void VulkanSwapchain::Initialize()
{
	CreateSwapchain();
	CacheSwapchainImages();
	InitializeRenderTargets();
	InitializeSwapchainSyncronization();
}

void VulkanSwapchain::Terminate()
{
}

void VulkanSwapchain::Recreate()
{
	CreateSwapchain();
	CacheSwapchainImages();
	InitializeRenderTargets();
}

VkResult VulkanSwapchain::GetNextImage()
{
	return vkAcquireNextImageKHR(logicalDevice.GetNativeHandle(), swapchainHandle, UINT64_MAX, imageAvailable, VK_NULL_HANDLE, &imageIndex);
}

void VulkanSwapchain::SubmitFrame()
{
	VulkanCommandBufferManager& cmdBufferManager = logicalDevice.GetCmdBufferManager();
	//cmdBufferManager.GetActiveGraphicsBuffer()->EndRenderPass();

	VkImageSubresourceRange range = {};
	range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	range.baseArrayLayer = 0;
	range.layerCount = 1;
	range.baseMipLevel = 0;
	range.levelCount = 1;
	ImageLayoutTransition(
		*cmdBufferManager.GetActiveGraphicsBuffer(), range,
		VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
		{ swapchainImageTargets[imageIndex]->image }
	);
	VkSemaphore graphicsWait = imageAvailable;
// 	if (cmdBufferManager.HasValidTransferBuffer())
// 	{
// 		logicalDevice->GetCmdBufferManager().SubmitTransferBuffer(false, imageAvailable, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, copyFinished);
// 		graphicsWait = copyFinished;
// 	}
	cmdBufferManager.SubmitGraphicsBuffer(true, graphicsWait, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, renderingFinished);
}

void VulkanSwapchain::Present()
{
	VulkanQueue* presentQueue = logicalDevice.GetGraphicsQueue();

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &renderingFinished;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapchainHandle;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr;

	VkResult result = vkQueuePresentKHR(presentQueue->GetNativeHandle(), &presentInfo);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
	{
		//Recreate()
	}
	else if (result != VK_SUCCESS)
	{
		// TODO - Log
		Assert(false);
	}
}

RenderTargetDescription VulkanSwapchain::GetSwapchainImageDescription() const
{
	return targetDescription;
}

RenderTargetTextures VulkanSwapchain::GetSwapchainTarget() const
{
	RenderTargetTextures screenTargets = {};
	screenTargets.colorTargets[0] = swapchainImageTargets[imageIndex];
	//screenTargets.depthTarget = swapchainImageTargets.Last();
	screenTargets.targetCount = 1;
	return screenTargets;
}

void VulkanSwapchain::CreateSwapchain()
{
	VkSurfaceCapabilitiesKHR surfaceCapabilites = surface->GetSurfaceCapabilities();
	DynamicArray<VkSurfaceFormatKHR> surfaceFormats = surface->GetSurfaceFormats();
	DynamicArray<VkPresentModeKHR> presentModes = surface->GetPresentModes();

	uint32 swapchainImageCount = surfaceCapabilites.minImageCount + 1;
	if (surfaceCapabilites.maxImageCount > 0 &&
		swapchainImageCount > surfaceCapabilites.maxImageCount)
	{
		swapchainImageCount = surfaceCapabilites.maxImageCount;
	}

	// Finding optimal supported surface format
	VkSurfaceFormatKHR surfaceFormat = {};
	if (surfaceFormats.Size() == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED)
	{
		surfaceFormat = {
			VK_FORMAT_R8G8B8A8_UNORM,
			VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
		};
	}
	else
	{
		// TODO - Add for-each support to my arrays
		for (uint32 i = 0; i < surfaceFormats.Size(); ++i)
		{
			if (surfaceFormats[i].format == VK_FORMAT_R8G8B8A8_UNORM)
			{
				surfaceFormat = surfaceFormats[i];
				break;
			}
		}
	}

	if (surfaceFormat.format != VK_FORMAT_R8G8B8A8_UNORM)
	{
		surfaceFormat = surfaceFormats[0];
	}
	swapchainFormat = surfaceFormat.format;

	// Set up swapchain extents
	swapchainExtent = { static_cast<uint32>(surface->GetSurfaceWidth()), static_cast<uint32>(surface->GetSurfaceHeight()) };
	if (surfaceCapabilites.currentExtent.width != std::numeric_limits<uint32>::max())
	{
		swapchainExtent = surfaceCapabilites.currentExtent;
	}
	else
	{
		swapchainExtent.width = std::max(surfaceCapabilites.minImageExtent.width, std::min(surfaceCapabilites.maxImageExtent.width, swapchainExtent.width));
		swapchainExtent.height = std::max(surfaceCapabilites.minImageExtent.height, std::min(surfaceCapabilites.maxImageExtent.height, swapchainExtent.height));
	}

	// Set up swapchain usage
	VkImageUsageFlags usageFlags;
	if (surfaceCapabilites.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT)
	{
		usageFlags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	}
	else
	{
		usageFlags = static_cast<VkImageUsageFlags>(-1);
	}

	// Setting up the surface transform. Mostly going to be using current transform
	VkSurfaceTransformFlagBitsKHR transformBits = surfaceCapabilites.currentTransform;

	// Set up presentation mode
	VkPresentModeKHR presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
	for (uint32 i = 0; i < presentModes.Size(); ++i)
	{
		// Only checking against the mode with lowest latency and still has V-Sync
// 		if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
// 		{
// 			presentMode = presentModes[i];
// 			break;
// 		}
		if(presentModes[i] == VK_PRESENT_MODE_FIFO_KHR)
		{
			presentMode = presentModes[i];
		}
	}

	VkSwapchainCreateInfoKHR swapchainInfo = {};
	swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainInfo.surface = surface->GetNativeHandle();
	swapchainInfo.minImageCount = swapchainImageCount;
	swapchainInfo.imageFormat = surfaceFormat.format;
	swapchainInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapchainInfo.imageExtent = swapchainExtent;
	swapchainInfo.imageArrayLayers = 1;
	swapchainInfo.imageUsage = usageFlags;
	swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchainInfo.queueFamilyIndexCount = 0;
	swapchainInfo.pQueueFamilyIndices = nullptr;
	swapchainInfo.preTransform = transformBits;
	swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainInfo.presentMode = presentMode;
	swapchainInfo.clipped = VK_TRUE;
	swapchainInfo.oldSwapchain = swapchainHandle;

	CHECK_VK(vkCreateSwapchainKHR(logicalDevice.GetNativeHandle(), &swapchainInfo, nullptr, &swapchainHandle));
}

void VulkanSwapchain::CacheSwapchainImages()
{
	uint32 imageCount;
	vkGetSwapchainImagesKHR(logicalDevice.GetNativeHandle(), swapchainHandle, &imageCount, nullptr);
	swapchainImages.Resize(imageCount);
	vkGetSwapchainImagesKHR(logicalDevice.GetNativeHandle(), swapchainHandle, &imageCount, swapchainImages.GetData());

	swapchainImageTargets.Resize(imageCount);
	for (uint32 i = 0; i < imageCount; ++i)
	{
		VulkanImage* img = new VulkanImage;
		img->device = &logicalDevice;
		img->handle = swapchainImages[i];
		img->width = swapchainExtent.width;
		img->height = swapchainExtent.height;
		img->format = swapchainFormat;
		img->layout = VK_IMAGE_LAYOUT_UNDEFINED;
		img->aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
		swapchainImageTargets[i] = new VulkanTexture(*img);
	}
}

void VulkanSwapchain::InitializeRenderTargets()
{
	targetDescription.targetCount = 1;
	targetDescription.targetExtents = { (float32)swapchainExtent.width, (float32)swapchainExtent.height };

	ColorDescription& colorDesc = targetDescription.colorDescs[0];
	colorDesc.format = ImageFormat::RGBA_8norm;
	colorDesc.load = LoadOperation::Clear;
	colorDesc.store = StoreOperation::Store;
	colorDesc.stencilLoad = LoadOperation::DontCare;
	colorDesc.stencilStore = StoreOperation::DontCare;
	colorDesc.sampleCount = 1;

	targetDescription.hasDepth = false;

// 	DepthStencilDescription& depthDesc = targetDescription.depthDesc;
// 	depthDesc.format = ImageFormat::DS_32f_8u;
// 	depthDesc.load = LoadOperation::Clear;
// 	depthDesc.store = StoreOperation::Store;
// 	depthDesc.stencilLoad = LoadOperation::DontCare;
// 	depthDesc.stencilStore = StoreOperation::DontCare;
// 	depthDesc.sampleCount = 1;
}

void VulkanSwapchain::InitializeSwapchainSyncronization()
{
	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	CHECK_VK(vkCreateSemaphore(logicalDevice.GetNativeHandle(), &semaphoreInfo, nullptr, &imageAvailable));
	CHECK_VK(vkCreateSemaphore(logicalDevice.GetNativeHandle(), &semaphoreInfo, nullptr, &renderingFinished));
}
