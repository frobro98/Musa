#pragma once

#include "VulkanAbstractions.h"
#include "Containers/DynamicArray.hpp"

struct Vertex;
struct Face;

class VulkanDevice;
class VulkanVertexBuffer;
class VulkanIndexBuffer;
class VulkanUniformBuffer;

class VulkanMemory
{
public:
	static void Initialize(VulkanDevice* device);

// 	static VulkanImage* AllocateImage(
// 		uint32 imgWidth, uint32 imgHeight, VkFormat imgFormat,
// 		uint32 mipMapLevels, VkImageTiling tiling, VkImageUsageFlags usage,
// 		VkMemoryPropertyFlags memoryProperties
// 	);
// 	static void DeallocateImage(
// 		VulkanImage* image
// 	);

// 	static VulkanBuffer* AllocateBuffer(
// 		VkDeviceSize bufferSize, VkBufferUsageFlags usageFlags,
// 		VkMemoryPropertyFlags memoryFlags
// 	);


	// TODO - Could be viewed as some sort of hack to get at the logical device. Need to probably refactor
// 	static void* MapMemory(VkDeviceMemory memory, VkDeviceSize memorySize);
// 	static void UnmapMemory(VkDeviceMemory memory);

private:
	static uint32 QueryMemoryType(uint32 typeFilter, VkMemoryPropertyFlags propertyFlags);

	VulkanMemory() = default;
	~VulkanMemory() = default;

	static VulkanMemory& Instance();

private:
	VulkanDevice* logicalDevice = nullptr;

};
