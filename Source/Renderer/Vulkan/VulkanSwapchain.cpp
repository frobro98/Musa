
#include "Containers/Array.h"

#include "VulkanSwapchain.h"
#include "VulkanDevice.h"
#include "VulkanFramebuffer.h"
#include "VulkanSurface.h"
#include "VulkanCreateInfos.h"
#include "VulkanRenderPass.h"
#include "VulkanMemory.h"
#include "VulkanUtilites.h"
#include "VulkanCommandBuffer.h"
#include "VulkanQueue.h"

#if GRAPHICS_API_VK

VulkanSwapchain::VulkanSwapchain(VulkanDevice * device, VulkanSurface* renderSurface)
	:logicalDevice(device), surface(renderSurface)
{
}

VulkanSwapchain::~VulkanSwapchain()
{
	// TODO - Need to clean up semaphores and fences
	assert(swapchainHandle != VK_NULL_HANDLE);
	vkDestroySwapchainKHR(logicalDevice->GetNativeHandle(), swapchainHandle, nullptr);
}

void VulkanSwapchain::Initialize()
{
	CreateSwapchain();
	CreateSwapchainImageViews();
	CreateRenderPass();
	CreateDepthView();
	CreateSwapchainFramebuffers();
	InitializeSwapchainSyncronization();
}

void VulkanSwapchain::Terminate()
{
}

void VulkanSwapchain::Recreate()
{
	CreateSwapchain();
	CreateSwapchainImageViews();
	CreateRenderPass();
	CreateDepthView();
	CreateSwapchainFramebuffers();
}

VkResult VulkanSwapchain::GetNextImage()
{
	return vkAcquireNextImageKHR(logicalDevice->GetNativeHandle(), swapchainHandle, UINT64_MAX, imageAvailable, VK_NULL_HANDLE, &imageIndex);
}

void VulkanSwapchain::SubmitFrame()
{
	VulkanCommandBuffer* graphicsCommandBuffer = VulkanCommandBufferManager::GetActiveGraphicsBuffer();
	assert(graphicsCommandBuffer->GetBufferState() == VulkanCommandBuffer::State::Began);
	VulkanCommandBuffer* copyCommandBuffer = VulkanCommandBufferManager::GetActiveTransferBuffer();
	VulkanCommandBuffer* computeCommandBuffer = VulkanCommandBufferManager::GetActiveComputeBuffer();
	

	VulkanQueue* graphicsQueue = logicalDevice->GetGraphicsQueue();
	VulkanQueue* copyQueue = logicalDevice->GetTransferQueue();
	VulkanQueue* computeQueue = logicalDevice->GetComputeQueue();


	if (copyCommandBuffer->GetBufferState() == VulkanCommandBuffer::State::Began)
	{
		VulkanSyncInfo copySync;
		copySync.waitSemaphore = imageAvailable;
		copySync.waitMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		copySync.signalSemaphore = copyFinished;
		copyCommandBuffer->End();
		copyCommandBuffer->Submit(copyQueue, &copySync);
	}

//#if 0 
	VulkanSyncInfo computeSync = {};
	computeSync.waitSemaphore = copyCommandBuffer->GetBufferState() == VulkanCommandBuffer::State::Began ? copyFinished : imageAvailable;
	computeSync.waitMask = VK_PIPELINE_STAGE_TRANSFER_BIT;
	computeSync.waitFence = waitCompute;
	computeSync.signalSemaphore = computeFinished;

	VulkanSyncInfo graphicsSync;
	graphicsSync.waitSemaphore = computeCommandBuffer->GetBufferState() == VulkanCommandBuffer::State::Began ? computeFinished : imageAvailable;
	graphicsSync.waitMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	graphicsSync.signalSemaphore = renderingFinished;

	if (computeCommandBuffer->GetBufferState() == VulkanCommandBuffer::State::Began)
	{
		computeCommandBuffer->End();
		computeCommandBuffer->Submit(computeQueue, &computeSync);

		//#endif
		vkWaitForFences(logicalDevice->GetNativeHandle(), 1, &waitCompute, VK_TRUE, UINT64_MAX);
		vkResetFences(logicalDevice->GetNativeHandle(), 1, &waitCompute);
	}

	graphicsCommandBuffer->End();
	graphicsCommandBuffer->Submit(graphicsQueue, &graphicsSync);	

	vkQueueWaitIdle(graphicsQueue->GetNativeHandle());
}

void VulkanSwapchain::Present()
{
	VulkanQueue* presentation = logicalDevice->GetPresentationQueue();

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &renderingFinished;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapchainHandle;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr;

	VkResult result = vkQueuePresentKHR(presentation->GetNativeHandle(), &presentInfo);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
	{
		//Recreate()
	}
	else if (result != VK_SUCCESS)
	{
		// TODO - Log
		assert(false);
	}
}

void VulkanSwapchain::CreateSwapchain()
{
	VkSurfaceCapabilitiesKHR surfaceCapabilites = surface->GetSurfaceCapabilities();
	Array<VkSurfaceFormatKHR> surfaceFormats = surface->GetSurfaceFormats();
	Array<VkPresentModeKHR> presentModes = surface->GetPresentModes();

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
		if (presentModes[i] == VK_PRESENT_MODE_FIFO_KHR)
		{
			presentMode = presentModes[i];
			break;
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

	CHECK_VK(vkCreateSwapchainKHR(logicalDevice->GetNativeHandle(), &swapchainInfo, nullptr, &swapchainHandle));
}

void VulkanSwapchain::CreateSwapchainImageViews()
{
	uint32 imageCount;
	vkGetSwapchainImagesKHR(logicalDevice->GetNativeHandle(), swapchainHandle, &imageCount, nullptr);
	Array<VkImage> swapchainImages(imageCount);
	vkGetSwapchainImagesKHR(logicalDevice->GetNativeHandle(), swapchainHandle, &imageCount, swapchainImages.GetData());

	swapchainImageViews.Resize(imageCount);
	for (uint32 i = 0; i < imageCount; ++i)
	{
		swapchainImageViews[i] = Vk::create::ImageView(
			logicalDevice,
			swapchainImages[i],
			swapchainFormat,
			VK_IMAGE_ASPECT_COLOR_BIT
			);
	}
}

void VulkanSwapchain::CreateRenderPass()
{
	renderPass = new VulkanRenderPass(logicalDevice, swapchainFormat);
}

void VulkanSwapchain::CreateDepthView()
{
	VkFormat depthFormat = renderPass->GetDepthFormat();
	depthImage = VulkanMemory::AllocateImage(
		swapchainExtent.width, swapchainExtent.height, 
		depthFormat, 1, VK_IMAGE_TILING_OPTIMAL, 
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, 
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
	);

	VkImageSubresourceRange subresourceRange = {};
	subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
	subresourceRange.baseArrayLayer = 0;
	subresourceRange.layerCount = 1;
	subresourceRange.baseMipLevel = 0;
	subresourceRange.levelCount = 1;

	// Image transition
	depthImage = ImageLayoutTransition(depthImage, subresourceRange, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

	depthImageView = Vk::create::ImageView(logicalDevice, depthImage.handle, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
}

void VulkanSwapchain::CreateSwapchainFramebuffers()
{
	swapchainFramebuffers.Reserve(swapchainImageViews.Size());
	for (uint32 i = 0; i < swapchainImageViews.Size(); ++i)
	{
		VkImageView framebufferAttachments[] = {
			swapchainImageViews[i],
			depthImageView
		};

		VulkanFramebuffer* frameBuffer = new VulkanFramebuffer(logicalDevice);
		frameBuffer->Initialize(framebufferAttachments, swapchainExtent, renderPass);
		swapchainFramebuffers.Add(std::move(frameBuffer));
	}
}

void VulkanSwapchain::InitializeSwapchainSyncronization()
{
	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	CHECK_VK(vkCreateSemaphore(logicalDevice->GetNativeHandle(), &semaphoreInfo, nullptr, &imageAvailable));
	CHECK_VK(vkCreateSemaphore(logicalDevice->GetNativeHandle(), &semaphoreInfo, nullptr, &copyFinished));
	CHECK_VK(vkCreateSemaphore(logicalDevice->GetNativeHandle(), &semaphoreInfo, nullptr, &computeFinished));
	CHECK_VK(vkCreateSemaphore(logicalDevice->GetNativeHandle(), &semaphoreInfo, nullptr, &renderingFinished));

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

	CHECK_VK(vkCreateFence(logicalDevice->GetNativeHandle(), &fenceInfo, nullptr, &waitCompute));
}

#endif