#pragma once

#include "Types/Intrinsics.hpp"
#include "MemoryUtilities.h"
#include "VulkanDevice.h"

struct VulkanBuffer;

constexpr size_t AllocationSizeBytes = MegabytesAsBytes(1);

struct TempAlloc
{
	void* allocData;
	VkBuffer buffer;
	VkDeviceSize allocSize;
	uint32 offset;
};

struct VulkanFrameTempAllocation
{
	VulkanFrameTempAllocation(const VulkanDevice& device);
	~VulkanFrameTempAllocation();

	TempAlloc AllocateTempMemory(VkDeviceSize allocSize, uint32 allocAlignment);
	void ClearSuballocations();

	void* freeAllocPtr;
	void* mappedBeginningPtr;
	VkDeviceSize allocationSize;

private:
	const VulkanDevice& logicalDevice;
	VulkanBuffer* allocBuffer;
};