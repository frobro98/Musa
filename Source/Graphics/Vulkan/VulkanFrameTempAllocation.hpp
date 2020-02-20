#pragma once

#include "Types/Intrinsics.hpp"
#include "MemoryUtilities.h"
#include "VulkanDevice.h"

struct VulkanBuffer;

constexpr uint32 AllocationSizeBytes = MegabytesAsBytes(1);

struct TempAlloc
{
	void* allocData;
	VkBuffer buffer;
	uint32 allocSize;
	uint32 offset;
};

struct VulkanFrameTempAllocation
{
	VulkanFrameTempAllocation(const VulkanDevice& device);
	~VulkanFrameTempAllocation();

	TempAlloc AllocateTempMemory(uint32 allocSize, uint32 allocAlignment);
	void ClearSuballocations();

	void* freeAllocPtr;
	void* mappedBeginningPtr;
	uint32 allocationSize;

private:
	const VulkanDevice& logicalDevice;
	VulkanBuffer* allocBuffer;
};