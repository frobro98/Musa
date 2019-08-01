
#include "VulkanCreateInfos.h"
#include "VulkanDevice.h"
#include "VulkanUtilities.h"
#include "EngineCore/Assertion.h"
#include "Graphics/ResourceInitializationDescriptions.hpp"


namespace Vk
{
	VkInstanceCreateInfo InstanceInfo(const tchar* const* instanceLayers, uint32 numLayers, const tchar* const* instanceExtensions, uint32 numExtensions, [[maybe_unused]] const void * additionalData)
	{
		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
#ifdef _DEBUG
		createInfo.pNext = additionalData;
#endif
		createInfo.enabledLayerCount = numLayers;
		createInfo.ppEnabledLayerNames = instanceLayers;
		createInfo.enabledExtensionCount = numExtensions;
		createInfo.ppEnabledExtensionNames = instanceExtensions;
		return createInfo;
	}

	VkDeviceQueueCreateInfo DeviceQueueInfo(uint32 queueFamilyIndex, uint32 numQueues, const float32 * queuePriorities)
	{
		VkDeviceQueueCreateInfo queueInfo = {};
		queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.queueFamilyIndex = queueFamilyIndex;
		queueInfo.queueCount = numQueues;
		queueInfo.pQueuePriorities = queuePriorities;
		return queueInfo;
	}

	VkDeviceCreateInfo DeviceInfo(const VkDeviceQueueCreateInfo * queueInfo, uint32 numQueueInfos, const tchar ** deviceExtensions, uint32 numExtensions, const VkPhysicalDeviceFeatures & deviceFeatures)
	{
		VkDeviceCreateInfo deviceInfo = {};
		deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceInfo.queueCreateInfoCount = numQueueInfos;
		deviceInfo.pQueueCreateInfos = queueInfo;
		deviceInfo.enabledExtensionCount = numExtensions;
		deviceInfo.ppEnabledExtensionNames = deviceExtensions;
		deviceInfo.pEnabledFeatures = &deviceFeatures;
		return deviceInfo;
	}

//#ifdef _DEBUG
	VkDebugReportCallbackCreateInfoEXT DebugReportCallbackInfo(PFN_vkDebugReportCallbackEXT debugFunc, VkDebugReportFlagsEXT debugFlags, void * userData)
	{
		VkDebugReportCallbackCreateInfoEXT createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		createInfo.pfnCallback = debugFunc;
		createInfo.flags = debugFlags;
		createInfo.pUserData = userData;
		return createInfo;
	}
//#endif // _DEBUG
#ifdef _WIN32
	VkWin32SurfaceCreateInfoKHR SurfaceInfo(HINSTANCE hInstance, HWND hWnd)
	{
		VkWin32SurfaceCreateInfoKHR surfaceInfo = {};
		surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceInfo.hinstance = hInstance;
		surfaceInfo.hwnd = hWnd;
		return surfaceInfo;
	}
#endif

	VkImageViewCreateInfo ImageViewInfo(VkImage image, uint32 mipLevels, VkFormat format, VkImageAspectFlags aspectFlags)
	{
		VkImageViewCreateInfo imageViewInfo = {};
		imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.image = image;
		imageViewInfo.format = format;
		imageViewInfo.subresourceRange.aspectMask = aspectFlags;
		imageViewInfo.subresourceRange.baseArrayLayer = 0;
		imageViewInfo.subresourceRange.layerCount = 1;
		imageViewInfo.subresourceRange.baseMipLevel = 0;
		imageViewInfo.subresourceRange.levelCount = mipLevels;
		return imageViewInfo;
	}

	VkRenderPassCreateInfo RenderPassInfo(const VkAttachmentDescription * attachments,
		uint32 numAttachments,
		const VkSubpassDescription * subpasses,
		uint32 numSubpasses,
		const VkSubpassDependency * dependencies,
		uint32 numDependencies)
	{
		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = numAttachments;
		renderPassInfo.pAttachments = attachments;
		renderPassInfo.subpassCount = numSubpasses;
		renderPassInfo.pSubpasses = subpasses;
		renderPassInfo.dependencyCount = numDependencies;
		renderPassInfo.pDependencies = dependencies;

		return renderPassInfo;
	}

	VkSamplerCreateInfo SamplerInfo(const SamplerDescription& params)
	{
		VkSamplerCreateInfo samplerInfo = {};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = MusaFilterToVkFilter(params.filter);
		samplerInfo.minFilter = MusaFilterToVkFilter(params.filter);
		samplerInfo.addressModeU = MusaAddressModeToVkAddressMode(params.addrModeU);
		samplerInfo.addressModeV = MusaAddressModeToVkAddressMode(params.addrModeV);
		samplerInfo.addressModeW = MusaAddressModeToVkAddressMode(params.addrModeU);
		samplerInfo.anisotropyEnable = params.enabledAnisotropy;
		samplerInfo.maxAnisotropy = params.maxAnisotropy;
		samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = MusaMipModeToVkMipMode(params.mipMode);
		samplerInfo.mipLodBias = params.mipLodBias;
		samplerInfo.minLod = params.minLod;
		samplerInfo.maxLod = params.maxLod;

		return samplerInfo;
	}
}

