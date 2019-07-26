
#include "VulkanMemoryManager.hpp"
#include "VulkanBufferAllocation.hpp"
#include "VulkanImageAllocation.hpp"
#include "VulkanDevice.h"

#include "Utilities/CoreUtilities.hpp"


VulkanMemoryManager::VulkanMemoryManager(VulkanDevice& device)
	: logicalDevice(device)
{
}

VulkanMemoryManager::~VulkanMemoryManager()
{
}

VulkanImage* VulkanMemoryManager::AllocateImage(
	uint32 imgWidth, uint32 imgHeight, VkFormat imgFormat,
	uint32 mipMapLevels, VkImageTiling tiling, VkImageUsageFlags usage,
	VkMemoryPropertyFlags memoryProperties
)
{
	const VkPhysicalDeviceMemoryProperties& properties = logicalDevice.GetMemoryProperties();

	// TODO - Figure out if I should move this create info where all the others live
	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.format = imgFormat;
	imageInfo.extent = { imgWidth, imgHeight, 1 };
	imageInfo.arrayLayers = 1;
	imageInfo.mipLevels = mipMapLevels;
	imageInfo.tiling = tiling;
	imageInfo.usage = usage;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkImage img;
	CHECK_VK(vkCreateImage(logicalDevice.GetNativeHandle(), &imageInfo, nullptr, &img));

	VkMemoryRequirements memoryRequirements;
	vkGetImageMemoryRequirements(logicalDevice.GetNativeHandle(), img, &memoryRequirements);

	uint32 memoryTypeIndex = QueryMemoryType(memoryRequirements.memoryTypeBits, memoryProperties);
	bool canBeMapped = (properties.memoryTypes[memoryTypeIndex].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0;
	bool isCached = (properties.memoryTypes[memoryTypeIndex].propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) != 0;
	bool isCoherent = (properties.memoryTypes[memoryTypeIndex].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) != 0;

	ImageGraphicsAllocation* allocation = new ImageGraphicsAllocation(
		logicalDevice, 
		img, usage, 
		memoryProperties, 
		(uint32)memoryRequirements.size, 
		(uint32)memoryRequirements.alignment,
		memoryTypeIndex,
		canBeMapped,
		isCached,
		isCoherent
	);

	imageAllocations.Add(allocation);
	allocations.Add(allocation);

	ImageMemory* memory = nullptr;
	bool success = allocation->TrySelectMemoryRange(
		(uint32)memoryRequirements.size,
		(uint32)memoryRequirements.alignment,
		memory
	);
	Assert(success);

	VulkanImage* image = new VulkanImage;
	image->layout = VK_IMAGE_LAYOUT_UNDEFINED;
	image->format = imgFormat;
	image->width = imgWidth;
	image->height = imgHeight;
	image->mipmapLevels = mipMapLevels;
	image->handle = img;
	image->memory = memory;
	image->device = &logicalDevice;

	return image;
}

void VulkanMemoryManager::DeallocateImage(VulkanImage& image)
{
	UNUSED(image);
}

VulkanBuffer* VulkanMemoryManager::AllocateBuffer(
	VkDeviceSize bufferSize, VkBufferUsageFlags usageFlags,
	VkMemoryPropertyFlags memoryFlags
)
{
	VulkanBuffer* buffer = nullptr;

	const VkPhysicalDeviceLimits& limits = logicalDevice.GetDeviceLimits();
	uint32 alignment = 1;
	// TODO - Helper for bit flag testing validity
	if ((usageFlags && VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT) != 0)
	{
		alignment = Max((uint32)limits.minUniformBufferOffsetAlignment, alignment);
	}
	// TODO - Helper for bit flag testing validity
	if ((usageFlags && VK_BUFFER_USAGE_STORAGE_BUFFER_BIT) != 0)
	{
		alignment = Max((uint32)limits.minStorageBufferOffsetAlignment, alignment);
	}
	
	uint32 buffSize = (uint32)bufferSize;
	if (!TryCreateBufferFrom(usedBufferAllocations, buffSize, alignment, usageFlags, memoryFlags, buffer))
	{
		if (!TryCreateBufferFrom(freeBufferAllocations, buffSize, alignment, usageFlags, memoryFlags, buffer))
		{
			buffer = CreateNewAllocation(buffSize, alignment, usageFlags, memoryFlags);
		}
	}

	Assert(buffer != nullptr);

	return buffer;
}

void VulkanMemoryManager::DeallocateBuffer(VulkanBuffer& buffer)
{
	UNUSED(buffer);
}

void VulkanMemoryManager::FreePendingAllocations()
{
}

VulkanBuffer* VulkanMemoryManager::CreateNewAllocation(uint32 bufferSize, uint32 alignment, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryFlags)
{
	BufferGraphicsAllocation* allocation = &AllocateBufferBlock(bufferSize, usageFlags, memoryFlags);
	usedBufferAllocations.Add(allocation);

	uint32 suballocationAlignment = Max(alignment, allocation->GetAlignment());

	BufferMemory* memory;
	bool success = allocation->TrySelectMemoryRange(bufferSize, suballocationAlignment, memory);
	Assert(success);

	return new VulkanBuffer(allocation->buffer, *memory);
}

uint32 VulkanMemoryManager::QueryMemoryType(uint32 typeFilter, VkMemoryPropertyFlags propertyFlags)
{
	uint32 typeIndex = 0;
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(logicalDevice.GetPhysicalDevice(), &memProperties);
	for (uint32 i = 0; i < memProperties.memoryTypeCount; ++i)
	{
		if ((typeFilter & (1 << i)) &&
			(memProperties.memoryTypes[i].propertyFlags & propertyFlags) == propertyFlags)
		{
			typeIndex = i;
		}
	}

	return typeIndex;
}

BufferGraphicsAllocation& VulkanMemoryManager::AllocateBufferBlock(uint32 bufferSize, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryFlags)
{
	const VkPhysicalDeviceMemoryProperties& properties = logicalDevice.GetMemoryProperties();

	uint32 allocSize = Max(bufferSize, DefaultAllocationSize);

	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = allocSize;
	bufferInfo.usage = usageFlags;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkBuffer bufferHandle = VK_NULL_HANDLE;
	CHECK_VK(vkCreateBuffer(logicalDevice.GetNativeHandle(), &bufferInfo, nullptr, &bufferHandle));

	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(logicalDevice.GetNativeHandle(), bufferHandle, &memoryRequirements);

	uint32 memoryTypeIndex = QueryMemoryType(memoryRequirements.memoryTypeBits, memoryFlags);
	bool canBeMapped = (properties.memoryTypes[memoryTypeIndex].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0;
	bool isCached = (properties.memoryTypes[memoryTypeIndex].propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) != 0;
	bool isCoherent = (properties.memoryTypes[memoryTypeIndex].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) != 0;

	BufferGraphicsAllocation* allocation = new BufferGraphicsAllocation(
		logicalDevice, bufferHandle,
		usageFlags, memoryFlags,
		(uint32)memoryRequirements.size,
		(uint32)memoryRequirements.alignment,
		memoryTypeIndex,
		canBeMapped, isCached, isCoherent
	);

	allocations.Add(allocation);

	return *allocation;
}
