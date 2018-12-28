#pragma once

#include "Graphics.h"
#include "Containers/Array.h"

#if GRAPHICS_API_VK

class Renderer;
class Window;
class VulkanQueue;
class VulkanDescriptorPool;

class VulkanDevice
{
public:
	VulkanDevice() = delete;
	VulkanDevice(Renderer* renderLayer);
	~VulkanDevice();

	void Initialize(VkInstance inst, const Window* window);
	VkFormat QuerySupportedFormat(Array<VkFormat> possibleFormats, VkImageTiling tiling, VkFormatFeatureFlags features);

	inline VkPhysicalDeviceLimits GetDeviceLimits() const { return gpuProperties.limits; }
	// TODO - Move this and probably descriptor pools elsewhere
	inline VulkanDescriptorPool* GetActivePool() const { return descriptorPools[0]; }

	inline VkDevice GetNativeHandle() const { return vulkanDevice; }
	inline VkPhysicalDevice GetPhysicalDevice() const { return gpu; }
	inline Renderer* GetRenderContext() const { return renderer; }
	
	inline VulkanQueue* GetGraphicsQueue() const
	{
		assert(vulkanDevice != VK_NULL_HANDLE);
		assert(graphicsQueue != nullptr);
		return graphicsQueue;
	}

	inline VulkanQueue* GetPresentationQueue() const
	{
		assert(vulkanDevice != VK_NULL_HANDLE);
		assert(presentationQueue != nullptr);
		return presentationQueue;
	}

	inline VulkanQueue* GetTransferQueue() const
	{
		assert(vulkanDevice != VK_NULL_HANDLE);
		assert(transferQueue != nullptr);
		return transferQueue;
	}

	inline VulkanQueue* GetComputeQueue() const
	{
		assert(vulkanDevice != VK_NULL_HANDLE);
		assert(computeQueue != nullptr);
		return computeQueue;
	}
private:
	bool IsSuitableGPU(VkPhysicalDevice physicalDevice, const Window* renderWindow, uint32& graphicsIndex, uint32& presentIndex, uint32& transferIndex, uint32& computeIndex);

	VkPhysicalDeviceProperties gpuProperties;
	VkPhysicalDeviceFeatures enabledGPUFeatures;
	VkPhysicalDevice gpu = VK_NULL_HANDLE;
	VkDevice vulkanDevice = VK_NULL_HANDLE;

	Array<class VulkanDescriptorPool*> descriptorPools;

	Renderer* renderer = nullptr;
	VulkanQueue* graphicsQueue = nullptr;
	VulkanQueue* presentationQueue = nullptr;
	VulkanQueue* transferQueue = nullptr;
	VulkanQueue* computeQueue = nullptr;
	uint32 pad[1];
};

#endif