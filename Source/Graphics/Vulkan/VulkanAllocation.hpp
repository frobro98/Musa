#pragma once

#include <mutex>

#include "VulkanDefinitions.h"
#include "Containers/DynamicArray.hpp"
#include "Ranges.hpp"
#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/ResourceLockPtr.hpp"


class GraphicsAllocation
{
public:
	virtual ~GraphicsAllocation() = default;
};

template <class Suballoc>
class GraphicsMemoryAllocation : public GraphicsAllocation
{
public:
	GraphicsMemoryAllocation(
		VulkanDevice& device,
		VkMemoryPropertyFlags memProperties,
		VkDeviceSize size,
		uint32 alignment,
		uint32 typeIndex,
		bool canMap, bool cached, bool coherent
	);

	virtual ~GraphicsMemoryAllocation();

	bool TrySelectMemoryRange(VkDeviceSize requestedSize, uint32 alignment, Suballoc*& range);

	void LockForWrite(VkDeviceSize size, uint32 offset);
	void Unlock();

// 	void* MapData(VkDeviceSize size, uint32 offset);
// 	void UnmapData();

	inline uint32 GetTotalMemoryRanges() const { return freeAllocationBlocks.Size(); }
	inline uint32 GetAlignment() const { return offsetAlignment; }

	inline bool IsCoherent() const { return isCoherent; }
	inline bool IsCached() const { return isCached; }
	inline bool CanBeMapped() const { return canMapData; }
	inline bool IsMapped() const { return mappedData != nullptr; }
	void* GetMappedPtr() const
	{
		Assert(IsMapped());
		return mappedData;
	}

	inline VkDeviceMemory GetMemoryHandle() const { return graphicsMemory; }
	inline VkMemoryPropertyFlags GetMemoryProperties() const { return memoryProperties; }

protected:
	virtual Suballoc* CreateMemoryRange(VkDeviceSize actualSize, uint32 blockSize, uint32 alignedOffset, uint32 actualOffset) = 0;

protected:
	DynamicArray<URange> freeAllocationBlocks;
	DynamicArray<Suballoc*> suballocationBlocks; 
	VkDeviceMemory graphicsMemory;
	VulkanDevice& logicalDevice;

private:
	std::mutex allocationMutex;
	void* mappedData = nullptr;
	VkMemoryPropertyFlags memoryProperties;
	VkDeviceSize allocationSize;
	uint32 usedSpaceSize;
	uint32 offsetAlignment;
	// This is for accessing information about the heap this allocation is apart
	uint32 memoryTypeIndex;
	uint32 canMapData : 1;
	uint32 isCoherent : 1;
	uint32 isCached : 1;
};

template <class Alloc>
struct GraphicsMemoryRange
{
	GraphicsMemoryRange(
		Alloc& owningAlloc,
		VkDeviceSize allocSize,
		uint32 blockSize,
		uint32 alignedOffset,
		uint32 actualOffset
	);

	virtual ~GraphicsMemoryRange();

	void LockForWrite();
	void Unlock();

// 	void MapData();
// 	void UnmapData();

	inline bool CanBeMapped() const { return owningAllocation.CanBeMapped(); }
	inline bool IsMapped() const { return owningAllocation.IsMapped(); }
	void* GetMappedPtr() const
	{
		Assert(CanBeMapped() && IsMapped());
		return owningAllocation.GetMappedPtr();
	}

	Alloc& owningAllocation;
	const VkDeviceSize size;

	// This is the size of the entire block of memory requested by the suballocation
	// Could be the same as total size
	const uint32 blockSize;
	// Aligned offset for when Vk is interacting with the memory
	const uint32 alignedOffset;
	// Actual offset of memory within the actual allocation
	const uint32 actualOffset;
};

//////////////////////////////////////////////////////////////////////////
// GraphicsMemoryAllocation
//////////////////////////////////////////////////////////////////////////

template <class Suballoc>
GraphicsMemoryAllocation<Suballoc>::GraphicsMemoryAllocation(
	VulkanDevice& device,
	VkMemoryPropertyFlags memProperties,
	VkDeviceSize size,
	uint32 memAlignment,
	uint32 typeIndex,
	bool canMap, bool cached, bool coherent)
	: logicalDevice(device),
	memoryProperties(memProperties),
	allocationSize(size),
	usedSpaceSize(0),
	offsetAlignment(memAlignment),
	memoryTypeIndex(typeIndex),
	canMapData(canMap),
	isCoherent(coherent),
	isCached(cached)
{
	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = size;
	allocInfo.memoryTypeIndex = typeIndex;
	// TODO - URange is an interesting concept, but I need something more flexible to cover size_t, uint64, uint16, etc...
	URange range = { 0, (uint32)size };
	freeAllocationBlocks.Add(range);

	[[maybe_unused]] VkResult result = vkAllocateMemory(device.GetNativeHandle(), &allocInfo, nullptr, &graphicsMemory);
	CHECK_VK(result);
}

template<class Suballoc>
inline GraphicsMemoryAllocation<Suballoc>::~GraphicsMemoryAllocation()
{
	for (auto subAlloc : suballocationBlocks)
	{
		delete subAlloc;
	}
	if (IsMapped())
	{
		Unlock();
	}
	vkFreeMemory(logicalDevice.GetNativeHandle(), graphicsMemory, nullptr);
}

template<class Suballoc>
inline bool GraphicsMemoryAllocation<Suballoc>::TrySelectMemoryRange(VkDeviceSize requestedSize, uint32 alignment, Suballoc *& memoryRange)
{
	uint32 adjustedAlignment = Math::Max(alignment, offsetAlignment);
	for (uint32 i = 0; i < freeAllocationBlocks.Size(); ++i)
	{
		// TODO - Need to change this shit to be able to use size_t/VkDeviceSize by default instead of using uint32
		URange& allocRange = freeAllocationBlocks[i];
		uint32 actualOffset = allocRange.start;
		uint32 alignedOffset = Align(actualOffset, adjustedAlignment);
		uint32 offsetAdjustedDiff = alignedOffset - actualOffset;
		uint32 totalRequestedMemory = offsetAdjustedDiff + (uint32)requestedSize;
		if (totalRequestedMemory <= allocRange.size)
		{
			if (totalRequestedMemory < allocRange.size)
			{
				allocRange.start += totalRequestedMemory;
				allocRange.size -= totalRequestedMemory;
			}
			else // equal
			{
				freeAllocationBlocks.Remove(i);
			}

			memoryRange = CreateMemoryRange(requestedSize, totalRequestedMemory, alignedOffset, actualOffset);

			return true;
		}
	}

	return false;
}

template<class Suballoc>
inline void GraphicsMemoryAllocation<Suballoc>::LockForWrite(VkDeviceSize size, uint32 offset)
{
	Assert(CanBeMapped());
	Assert(!IsMapped());
	allocationMutex.lock();
	[[maybe_unused]] VkResult result = vkMapMemory(logicalDevice.GetNativeHandle(), graphicsMemory, offset, size, 0, &mappedData);
	CHECK_VK(result);
}

template<class Suballoc>
inline void GraphicsMemoryAllocation<Suballoc>::Unlock()
{
	Assert(CanBeMapped());
	Assert(IsMapped());
	vkUnmapMemory(logicalDevice.GetNativeHandle(), graphicsMemory);
	mappedData = nullptr;
	allocationMutex.unlock();
}

// template <class Suballoc>
// void* GraphicsMemoryAllocation<Suballoc>::MapData(VkDeviceSize size, uint32 offset)
// {
// 	Assert(CanBeMapped());
// 	Assert(!IsMapped());
// 	CHECK_VK(vkMapMemory(logicalDevice.GetNativeHandle(), graphicsMemory, offset, size, 0, &mappedData));
// 	return mappedData;
// }
// 
// template <class Suballoc>
// void GraphicsMemoryAllocation<Suballoc>::UnmapData()
// {
// 	Assert(CanBeMapped());
// 	Assert(IsMapped());
// 	vkUnmapMemory(logicalDevice.GetNativeHandle(), graphicsMemory);
// 	mappedData = nullptr;
// }

//////////////////////////////////////////////////////////////////////////
// GraphicsMemoryRange
//////////////////////////////////////////////////////////////////////////

template <class Alloc>
GraphicsMemoryRange<Alloc>::GraphicsMemoryRange(
	Alloc& owningAlloc,
	VkDeviceSize allocSize,
	uint32 blckSize,
	uint32 alignedOffset,
	uint32 actualOffset
)
	: owningAllocation(owningAlloc),
	size(allocSize),
	blockSize(blckSize),
	alignedOffset(alignedOffset),
	actualOffset(actualOffset)
{
}

template<class Alloc>
inline GraphicsMemoryRange<Alloc>::~GraphicsMemoryRange()
{
}

template<class Alloc>
inline void GraphicsMemoryRange<Alloc>::LockForWrite()
{
	owningAllocation.LockForWrite(size, alignedOffset);
}

template<class Alloc>
inline void GraphicsMemoryRange<Alloc>::Unlock()
{
	owningAllocation.Unlock();
}
