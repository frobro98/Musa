#pragma once

#include "VulkanAllocation.hpp"

class ImageMemory;

struct ImageGraphicsAllocation : public GraphicsMemoryAllocation<ImageMemory>
{
	ImageGraphicsAllocation(
		VulkanDevice& device,
		VkImage image,
		VkImageUsageFlags usage,
		VkMemoryPropertyFlags memProperties,
		uint32 size,
		uint32 alignment,
		uint32 typeIndex,
		bool canMap, bool cached, bool coherent
	);

	inline VkImageUsageFlags GetImageUsage() const { return usageFlags; }
	inline VkImage GetImageHandle() const { return image; }

	VkImage image;
	VkImageUsageFlags usageFlags;

protected:
	virtual ImageMemory* CreateMemoryRange(VkDeviceSize actualSize, uint32 blockSize, uint32 alignedOffset, uint32 actualOffset) override;
	uint32 pad[1] = { 0 };
};

class ImageMemory : public GraphicsMemoryRange<ImageGraphicsAllocation>
{
public:
	ImageMemory(
		ImageGraphicsAllocation& owningAlloc, 
		VkImage image,
		VkDeviceSize size,
		uint32 blockSize, 
		uint32 alignedOffset, 
		uint32 actualOffset
	);

private:
	VkImage associatedImage;
};