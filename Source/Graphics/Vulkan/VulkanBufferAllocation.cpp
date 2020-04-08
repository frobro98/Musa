
#include "VulkanBufferAllocation.hpp"
#include "VulkanDevice.h"

BufferGraphicsAllocation::BufferGraphicsAllocation(
	VulkanDevice& device,
	VkBuffer buff, 
	VkBufferUsageFlags usage,
	VkMemoryPropertyFlags memProperties,
	uint32 size, 
	uint32 alignment,
	uint32 typeIndex, 
	bool canMap, bool cached, bool coherent)
	: GraphicsMemoryAllocation(
		device,
		memProperties, 
		size, 
		alignment,
		typeIndex, 
		canMap, cached, coherent),
	buffer(buff),
	usageFlags(usage)
{
	[[maybe_unused]] VkResult result = vkBindBufferMemory(device.GetNativeHandle(), buffer, graphicsMemory, 0);
	CHECK_VK(result);
}

BufferMemory* BufferGraphicsAllocation::CreateMemoryRange(VkDeviceSize bufferSize, uint32 blockSize, uint32 alignedOffset, uint32 actualOffset)
{
	return new BufferMemory(*this, buffer, bufferSize, blockSize, alignedOffset, actualOffset);
}

BufferMemory::BufferMemory(BufferGraphicsAllocation& owningAlloc, VkBuffer buffer, VkDeviceSize size, uint32 blockSize, uint32 alignedOffset, uint32 actualOffset)
	: GraphicsMemoryRange(owningAlloc, size, blockSize, alignedOffset, actualOffset),
	associatedBuffer(buffer)
{
}
