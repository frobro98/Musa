#pragma once

#include "Graphics.h"

class VulkanDevice;
struct TextureSamplerCreateParams;

namespace Vk
{
	VkInstanceCreateInfo InstanceInfo(
		const tchar* const* instanceLayers, uint32 numLayers,
		const tchar* const* instanceExtensions, uint32 numExtensions,
		const void* additionalData = nullptr
	);

	VkDeviceQueueCreateInfo DeviceQueueInfo(
		uint32 queueFamilyIndex, 
		uint32 numQueues, 
		const float32* queuePriorities
	);

	VkDeviceCreateInfo DeviceInfo(
		const VkDeviceQueueCreateInfo* queueInfo, uint32 numQueueInfos,
		const tchar** deviceExtensions, uint32 numExtensions,
		const VkPhysicalDeviceFeatures& deviceFeatures
	);

//#ifdef _DEBUG
	VkDebugReportCallbackCreateInfoEXT DebugReportCallbackInfo(
		PFN_vkDebugReportCallbackEXT debugFunc,
		VkDebugReportFlagsEXT debugFlags,
		void* userData = 0
	);
//#endif // _DEBUG

#ifdef _WIN32
	VkWin32SurfaceCreateInfoKHR SurfaceInfo(HINSTANCE hInstance, HWND hWnd);
#endif

	VkImageViewCreateInfo ImageViewInfo(
		VkImage image, uint32 mipLevels, VkFormat format, VkImageAspectFlags aspectFlags
	);

	VkRenderPassCreateInfo RenderPassInfo(
		const VkAttachmentDescription* attachments, uint32 numAttachments,
		const VkSubpassDescription* subpasses, uint32 numSubpasses,
		const VkSubpassDependency* dependencies = nullptr, uint32 numDependencies = 0
	);

	VkSamplerCreateInfo SamplerInfo(const TextureSamplerCreateParams& params);

	namespace create
	{
		VkImageView ImageView(
			const VulkanDevice* device, 
			VkImage image, 
			VkFormat format, 
			VkImageAspectFlags aspectFlags
		);
	}
}
