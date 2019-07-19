#pragma once

#include "VulkanAllocation.hpp"

class BufferMemory;

struct BufferGraphicsAllocation : public GraphicsMemoryAllocation<BufferMemory>
{
public:
	BufferGraphicsAllocation(
		VulkanDevice& device,
		VkBuffer buffer,
		VkBufferUsageFlags usage,
		VkMemoryPropertyFlags memProperties,
		uint32 size,
		uint32 alignment,
		uint32 typeIndex,
		bool canMap, bool cached, bool coherent
	);

	virtual ~BufferGraphicsAllocation();

	VkBuffer buffer;
	VkBufferUsageFlags usageFlags;

private:
	virtual BufferMemory* CreateMemoryRange(uint32 actualSize, uint32 blockSize, uint32 alignedOffset, uint32 actualOffset) override;
	uint32 pad[1] = { 0 };
};

class BufferMemory : public GraphicsMemoryRange<BufferGraphicsAllocation>
{
public:
	BufferMemory(
		BufferGraphicsAllocation& owningAlloc, 
		VkBuffer buffer,
		uint32 size,
		uint32 blockSize,
		uint32 alignedOffset,
		uint32 actualOffset
	);

private:
	VkBuffer associatedBuffer;
};