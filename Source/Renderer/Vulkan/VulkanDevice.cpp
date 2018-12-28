#include "VulkanDevice.h"
#include "VulkanCreateInfos.h"
#include "VulkanQueue.h"
#include "Windowing/Window.h"
#include "VulkanDescriptorPool.h"

#if GRAPHICS_API_VK

VulkanDevice::VulkanDevice(Renderer * renderLayer)
	:renderer(renderLayer)
{
	enabledGPUFeatures = {};
}

VulkanDevice::~VulkanDevice()
{
}

void VulkanDevice::Initialize(VkInstance inst, const Window* window)
{
	uint32 physicalDeviceCount = 0;
	CHECK_VK(vkEnumeratePhysicalDevices(inst, &physicalDeviceCount, nullptr));
	Array<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
	CHECK_VK(vkEnumeratePhysicalDevices(inst, &physicalDeviceCount, physicalDevices.GetData()));

	uint32 graphicsFamilyIndex = std::numeric_limits<uint32>::max();
	uint32 presentationFamilyIndex = std::numeric_limits<uint32>::max();
	uint32 transferFamilyIndex = std::numeric_limits<uint32>::max();
	uint32 computeFamilyIndex = std::numeric_limits<uint32>::max();
	for(const auto& physicalDevice : physicalDevices)
	{
		if (IsSuitableGPU(physicalDevice, window, graphicsFamilyIndex, presentationFamilyIndex, transferFamilyIndex, computeFamilyIndex))
		{
			gpu = physicalDevice;
			break;
		}
	}

	assert(graphicsFamilyIndex != std::numeric_limits<uint32>::max());
	assert(transferFamilyIndex != std::numeric_limits<uint32>::max());
	assert(presentationFamilyIndex != std::numeric_limits<uint32>::max());
	assert(computeFamilyIndex != std::numeric_limits<uint32>::max());

	float32 priorities[] = { 1.0f };
	VkDeviceQueueCreateInfo queueInfos[] =
	{
		Vk::DeviceQueueInfo(graphicsFamilyIndex, 1, priorities),
		Vk::DeviceQueueInfo(transferFamilyIndex, 1, priorities),
		Vk::DeviceQueueInfo(computeFamilyIndex, 1, priorities)
	};

	assert(presentationFamilyIndex == graphicsFamilyIndex);

	const tchar* deviceExtensions[] = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	// Enabling features
	VkPhysicalDeviceFeatures gpuFeatures;
	VkFormatProperties properties;
	vkGetPhysicalDeviceFormatProperties(gpu, VK_FORMAT_R8G8B8_UNORM, &properties);
	vkGetPhysicalDeviceFeatures(gpu, &gpuFeatures);
	vkGetPhysicalDeviceProperties(gpu, &gpuProperties);

	if (gpuFeatures.geometryShader)
	{
		enabledGPUFeatures.geometryShader = VK_TRUE;
	}
	if (gpuFeatures.tessellationShader)
	{
		enabledGPUFeatures.tessellationShader = VK_TRUE;
	}
	if (gpuFeatures.fillModeNonSolid)
	{
		enabledGPUFeatures.fillModeNonSolid = VK_TRUE;
	}
	if (gpuFeatures.textureCompressionBC)
	{
		enabledGPUFeatures.textureCompressionBC = VK_TRUE;
	}
	if (gpuFeatures.textureCompressionETC2)
	{
		enabledGPUFeatures.textureCompressionETC2 = VK_TRUE;
	}
	if (gpuFeatures.textureCompressionASTC_LDR)
	{
		enabledGPUFeatures.textureCompressionASTC_LDR = VK_TRUE;
	}

	assert(enabledGPUFeatures.fillModeNonSolid);
	assert(enabledGPUFeatures.tessellationShader);
	assert(enabledGPUFeatures.geometryShader);
	assert(enabledGPUFeatures.textureCompressionBC);

	VkDeviceCreateInfo deviceInfo = Vk::DeviceInfo(queueInfos, ArraySize(queueInfos), deviceExtensions, ArraySize(deviceExtensions), enabledGPUFeatures);
	vkCreateDevice(gpu, &deviceInfo, nullptr, &vulkanDevice);

	graphicsQueue = new VulkanQueue(this);
 	graphicsQueue->Initialize(graphicsFamilyIndex, VulkanQueue::Type::Graphics);

	presentationQueue = new VulkanQueue(this);
 	presentationQueue->Initialize(presentationFamilyIndex, VulkanQueue::Type::Present);

	transferQueue = new VulkanQueue(this);
	transferQueue->Initialize(transferFamilyIndex, VulkanQueue::Type::Copy);

	computeQueue = new VulkanQueue(this);
	computeQueue->Initialize(computeFamilyIndex, VulkanQueue::Type::Compute);

	descriptorPools.Add(new VulkanDescriptorPool(this));
}

VkFormat VulkanDevice::QuerySupportedFormat(Array<VkFormat> possibleFormats, VkImageTiling tiling, VkFormatFeatureFlags features)
{
	assert(vulkanDevice);

	VkFormat supportedFormat = VK_FORMAT_UNDEFINED;
	for (uint32 i = 0; i < possibleFormats.Size(); ++i)
	{
		VkFormat format = possibleFormats[i];
		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(gpu, format, &formatProperties);
		if (tiling == VK_IMAGE_TILING_OPTIMAL && (formatProperties.optimalTilingFeatures & features) == features)
		{
			supportedFormat = format;
			break;
		}
		if (tiling == VK_IMAGE_TILING_LINEAR && (formatProperties.linearTilingFeatures & features) == features)
		{
			supportedFormat = format;
			break;
		}
	}

	return supportedFormat;
}

bool VulkanDevice::IsSuitableGPU(VkPhysicalDevice physicalDevice, const Window* renderWindow, uint32& graphicsIndex, uint32& presentIndex, uint32& transferIndex, uint32& computeIndex)
{
	VkPhysicalDeviceProperties deviceProperties;

	vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
	{
		// TODO - Check if checking version of GPU api is necessary

		// TODO - Check what limits are important for a gpu
		assert(deviceProperties.limits.maxImageDimension2D >= 4096);

		uint32 queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
		assert(queueFamilyCount > 0);
		Array<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties.GetData());

		for (uint32 i = 0; i < queueFamilyCount; ++i)
		{
			VkBool32 presentationSupported = renderWindow->PresentationSupported(physicalDevice, i);

			if (queueFamilyProperties[i].queueCount > 0 &&
				queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				if (graphicsIndex == std::numeric_limits<uint32>::max())
				{
					graphicsIndex = i;
				}

				if (presentationSupported)
				{
					graphicsIndex = i;
					presentIndex = i;
				}
			}
			else if (queueFamilyProperties[i].queueCount > 0 &&
				queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
			{
				if (computeIndex == std::numeric_limits<uint32>::max())
				{
					computeIndex = i;
				}
			}
			else if (queueFamilyProperties[i].queueCount > 0 &&
				queueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
			{
				if (transferIndex == std::numeric_limits<uint32>::max())
				{
					transferIndex = i;
				}
			}
		}

		// No dedicated compute queue. This finds compute in any family
		if (computeIndex == std::numeric_limits<uint32>::max())
		{
			for (uint32 i = 0; i < queueFamilyCount; ++i)
			{
				if (queueFamilyProperties[i].queueCount > 0 &&
					queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
				{
					computeIndex = i;
					break;
				}
			}
		}

		assert(computeIndex < std::numeric_limits<uint32>::max());
		assert(presentIndex < std::numeric_limits<uint32>::max());

		return true;
	}
	else
	{
		// TODO - Log that there isn't support for any other type of GPU

		return false;
	}
}

#endif