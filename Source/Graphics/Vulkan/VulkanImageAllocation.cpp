
#include "VulkanImageAllocation.hpp"
#include "VulkanDevice.h"

ImageGraphicsAllocation::ImageGraphicsAllocation(
	VulkanDevice& device, 
	VkImage img,
	VkImageUsageFlags usage,
	VkMemoryPropertyFlags memProperties,
	uint32 size, 
	uint32 alignment,
	uint32 typeIndex, 
	bool canMap, bool cached, bool coherent
)
	: GraphicsMemoryAllocation(device,memProperties,size,alignment,typeIndex,canMap,cached,coherent),
	image(img),
	usageFlags(usage)
{
	NOT_USED VkResult result = vkBindImageMemory(device.GetNativeHandle(), image, graphicsMemory, 0);
	CHECK_VK(result);
}

ImageMemory* ImageGraphicsAllocation::CreateMemoryRange(VkDeviceSize actualSize, uint32 blockSize, uint32 alignedOffset, uint32 actualOffset)
{
	return new ImageMemory(*this, image, actualSize, blockSize, alignedOffset, actualOffset);
}

ImageMemory::ImageMemory(ImageGraphicsAllocation& owningAlloc, VkImage img, VkDeviceSize imgSize, uint32 blockSize, uint32 alignedOffset, uint32 actualOffset)
	: GraphicsMemoryRange(owningAlloc, imgSize, blockSize, alignedOffset, actualOffset),
	associatedImage(img)
{
}
