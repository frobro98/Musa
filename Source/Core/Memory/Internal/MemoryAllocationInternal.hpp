// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "Memory/MemoryCore.hpp"
#include "Threading/CriticalSection.hpp"
#include "Internal/MemoryFixedBlock.hpp"
#include "Internal/MemoryBlockInfoTable.hpp"
#include "Platform/PlatformMemory.hpp"
#include "Threading/ScopedLock.hpp"
//#include "Logging/LogCore.hpp"

// TODO - This will be replaced with logging
#include "Debugging/DebugOutput.hpp"

//DEFINE_LOG_CHANNEL(MemInternal);

namespace Memory::Internal
{
static void InitializeMemory();
static void DeinitializeMemory();

// Initialize Memory Core first thing
#pragma warning(disable:4075)
#pragma init_seg(".CRT$XCT")
static FixedBlockTableElement fixedBlockTable[TotalSmallFixedTableSizes];
static MemoryBlockInfoBucket* blockInfoHashTable = nullptr;
static u8 fixedSizeToIndexCache[FixedSizeCacheCount];

struct MemoryCoreController
{
	MemoryCoreController()
	{
		InitializeMemory();
	}
	~MemoryCoreController()
	{
		DeinitializeMemory();
	}
};

MemoryCoreController initObject;
#pragma warning(default:4075)

// TODO - This probably wants to be somewhere else because then we can have a better system around logging metrics
struct MemoryStats
{
	size_t allocatedFixedMemory = 0;
	size_t allocatedBigMemory = 0;
	size_t usedFixedMemory = 0;
	size_t usedBigMemory = 0;
};
static MemoryStats memoryStats;

static forceinline void InitializeFixedBlockTable()
{
	// Initialize small alloc table, currently using 128 size
	for (u32 i = 0; i < TotalSmallFixedTableSizes; ++i)
	{
		fixedBlockTable[i].fixedBlockSize = SmallFixedTableSizes[i];
		fixedBlockTable[i].blocksPerPage = GetAdjustedFreeBlocksFor(SmallFixedTableSizes[i]);
	}

	// initialize size -> table element array
	u8 tableIndex = 0;
	for (u16 i = 0; i < FixedSizeCacheCount; ++i)
	{
		// All sizes that come in will be aligned to 16 bytes, which then puts them into one of the fixed table elements
		u16 fixedBlockSize = i << BitsForDefaultAlignment;
		while (fixedBlockTable[tableIndex].fixedBlockSize < fixedBlockSize)
		{
			++tableIndex;
		}
		Assert(tableIndex < TotalSmallFixedTableSizes);
		fixedSizeToIndexCache[i] = tableIndex;
	}
}

static forceinline void InitializeMemoryInfoTable()
{

	// Apparently, VirtualAlloc allocates virtual pages when asking for memory less, or even more, than a single page.
	// However, it'll only fuck with virtual address space. So if I want to reserve and commit 4K, I do below...
	//constexpr size_t bucketMemorySize = Align(memoryBucketCount * sizeof(MemoryBlockInfoBucket), KilobytesAsBytes(4));
	blockInfoHashTable = (MemoryBlockInfoBucket*)PlatformMemory::PlatformAlloc(MemoryBucketCount * sizeof(MemoryBlockInfoBucket));
	Assert(blockInfoHashTable);

	for (u32 i = 0; i < MemoryBucketCount; ++i)
	{
		new(&blockInfoHashTable[i]) MemoryBlockInfoBucket;
	}
}

//////////////////////////////////////////////////////////////////////////
// Memory System Startup/Stop
//////////////////////////////////////////////////////////////////////////
static bool isInitialized = false;

static void InitializeMemory()
{
	Assert(!isInitialized);
	NOT_USED PlatformMemory::PlatformMemoryInfo memInfo = PlatformMemory::GetPlatformMemoryInfo();

	// Use memory info to generate however many allocation meta data buckets there are, forever

	InitializeFixedBlockTable();

	InitializeMemoryInfoTable();

	isInitialized = true;
}

static void DeinitializeMemory()
{
	Assert(isInitialized);

	// TODO - Replace these with a possible log output
	Debug::Printf("Big memory allocations: {}\n", memoryStats.allocatedBigMemory);
	Debug::Printf("Fixed memory allocations: {}\n", memoryStats.allocatedFixedMemory);

	isInitialized = false;
}

//////////////////////////////////////////////////////////////////////////
// Memory Block Info Accessors
//////////////////////////////////////////////////////////////////////////
static forceinline MemoryBlockInfo& InitializeOrFindMemoryInfo(void* p, BlockType blockType)
{
	// TODO - This might need some sort of synchronization
	// First thoughts are that it doesn't because the addresses coming from the OS
	// are all unique, so there wouldn't be any kind of issue accessing the same bucket.
	//
	// If there are, in fact, possible collisions that could happen because of "similar" addresses,
	// That kind of thing needs to be addressed. Might need a lock for that kind of thing.
	// Consult Hacky's implementation of NetherRealm's lock free behaviors and even lock
	// locations
	Assert(p);
	uptr addr = reinterpret_cast<uptr>(p);
	uptr preprocessedAddr = addr >> BitsInPageAllocation;
	u64 blockIndex = preprocessedAddr & BlockInfoMask;
	u64 memoryBucketIndex = (preprocessedAddr >> BitsOfBlockInfoMask) & MemoryBucketMask;

	// TODO - Make this parallel arrays of some kind
	MemoryBlockInfoBucket& bucket = blockInfoHashTable[memoryBucketIndex];
	if (bucket.blockInfo == nullptr)
	{
		bucket.blockInfo = (MemoryBlockInfo*)PlatformMemory::PlatformAlloc(PageAllocationSize);
		Assert(bucket.blockInfo);
		for (u32 i = 0; i < BlockInfosPerBucket; ++i)
		{
			new(&bucket.blockInfo[i]) MemoryBlockInfo;
		}
	}

	MemoryBlockInfo& blockInfo = bucket.blockInfo[blockIndex];
	blockInfo.flag = OwnedBlockFlag::YesBlock;
	if (blockInfo.type == BlockType::Untyped)
	{
		blockInfo.type = blockType;
	}
	else
	{
		Assert(blockInfo.type == blockType);
	}

	return blockInfo;
}


static forceinline void DeinitializeMemoryInfo(MemoryBlockInfo& info)
{
	info.allocatedSize = 0;
	info.flag = OwnedBlockFlag::NoBlock;
	info.type = BlockType::Untyped;
}

static forceinline MemoryBlockInfo* FindExistingMemoryInfo(void* p)
{
	uptr addr = reinterpret_cast<uptr>(p);
	Assert(addr > 0);

	uptr preprocessedAddr = addr >> BitsInPageAllocation;
	u32 blockIndex = preprocessedAddr & BlockInfoMask;
	u64 memoryBucketIndex = (preprocessedAddr >> BitsOfBlockInfoMask) & MemoryBucketMask;

	// TODO - Make this parallel arrays of some kind
	MemoryBlockInfoBucket& bucket = blockInfoHashTable[memoryBucketIndex];
	Assert(bucket.blockInfo);
	MemoryBlockInfo* blockInfo = nullptr;
	if (bucket.blockInfo)
	{
		blockInfo = &bucket.blockInfo[blockIndex];
		Assert(blockInfo->flag == OwnedBlockFlag::YesBlock);
		Assert(blockInfo->type != BlockType::Untyped);
	}

	return blockInfo;
}

//////////////////////////////////////////////////////////////////////////
// Fixed Block Size to Fixed Block Table Index
//////////////////////////////////////////////////////////////////////////
static forceinline u8 FixedSizeToTableIndex(size_t alignedSize)
{
	// Bump up size to next 16 byte aligned size. Index 0 and 1 are associated with 16, so
	// this needs to ignore 0 index, which means bump up size to next alignment boundary
	size_t fixedSizeCacheIndex = (alignedSize + AllocationDefaultAlignment - 1) >> BitsForDefaultAlignment;
	Assert(fixedSizeCacheIndex > 0 && fixedSizeCacheIndex <= (MaxFixedTableSize >> BitsForDefaultAlignment));
	u8 tableIndex = fixedSizeToIndexCache[fixedSizeCacheIndex];
	return tableIndex;
}

//////////////////////////////////////////////////////////////////////////
// Malloc Functions
//////////////////////////////////////////////////////////////////////////
forceinline void* MallocFixedBlock(size_t size)
{
	// Do fixed block allocation
	u8 tableIndex = FixedSizeToTableIndex(size);

	// TODO - Critical Section per table element
	FixedBlockTableElement& tableElement = fixedBlockTable[tableIndex];
	
	ScopedLock poolLock(tableElement.critSection);

	FixedBlockPool* pool = tableElement.availablePools;
	if (pool == nullptr)
	{
		// Create pool and link it to front and back
		pool = AllocateNewPoolFor(tableElement, tableIndex);
		memoryStats.allocatedFixedMemory += PageAllocationSize;

		MemoryBlockInfo& blockInfo = InitializeOrFindMemoryInfo(pool->nextFreeBlock, BlockType::FixedSmallBlock);
		blockInfo.allocatedSize = tableElement.fixedBlockSize;
	}
	Assert(pool);

	// Allocate block from fixed block pool
	void* p = AllocateFromFreedBlock(*pool);
	if (pool->blocksInUse == tableElement.blocksPerPage)
	{
		tableElement.availablePools = pool->next;
		pool->next = tableElement.emptyPools;
		pool->prev = nullptr;
		if (tableElement.emptyPools)
		{
			tableElement.emptyPools->prev = pool;
		}
		tableElement.emptyPools = pool;
	}
	memoryStats.usedFixedMemory += tableElement.fixedBlockSize;
	return p;
}

forceinline void* MallocLargeBlock(size_t size, size_t alignment)
{
	size_t alignedSize = Align(size, alignment);
	// Do big boi allocation from OS
	void* ret = PlatformMemory::PlatformAlloc(alignedSize);
	Assert(ret);
	MemoryBlockInfo& blockInfo = InitializeOrFindMemoryInfo(ret, BlockType::FixedSmallBlock);
	blockInfo.allocatedSize = alignedSize;

	memoryStats.allocatedBigMemory += alignedSize;
	memoryStats.usedBigMemory += alignedSize;

	return ret;
}

//////////////////////////////////////////////////////////////////////////
// Free Functions
//////////////////////////////////////////////////////////////////////////

// TODO - Cache a certain amount of pages to prevent OS free calls happening a bunch
forceinline void FreeFixedBlock(void* p)
{
	// We know now that this is a fixed block allocation. We need to get back to the main "FreedBlock" header
	FreedBlock* fixedBlockHeader = GetFixedBlockHeader(p);
	Assert(fixedBlockHeader->headerID == FreedBlock::BlockTag);
	u8 tableIndex = FixedSizeToTableIndex(fixedBlockHeader->blockSize);
	FixedBlockTableElement& tableElement = fixedBlockTable[tableIndex];

	FreedBlock* freeBlock = (FreedBlock*)p;
#if M_DEBUG
	Memfill(freeBlock, 0xdeaddead, fixedBlockHeader->blockSize);
#endif
	freeBlock->blockSize = fixedBlockHeader->blockSize;
	freeBlock->headerID = FreedBlock::BlockTag;
	freeBlock->numFreeBlocks = 1;
	freeBlock->nextBlock = nullptr;

	ScopedLock poolLock(tableElement.critSection);
	FixedBlockPool* pool = PushFreeBlockToPool(tableElement, freeBlock);
	Assert(pool);
	memoryStats.usedFixedMemory -= tableElement.fixedBlockSize;

	// If pool was previously empty, then move it to available list
	if (pool->blocksInUse == (tableElement.blocksPerPage - 1))
	{
		if (pool->prev)
		{
			pool->prev->next = pool->next;
		}
		if (pool->next)
		{
			pool->next->prev = pool->prev;
		}
		if (pool == tableElement.emptyPools)
		{
			tableElement.emptyPools = pool->next;
		}
		pool->next = tableElement.availablePools;
		pool->prev = nullptr;
		if (tableElement.availablePools)
		{
			tableElement.availablePools->prev = pool;
		}
		tableElement.availablePools = pool;
	}
	// If pool doesn't have any blocks in use, free it up baby
	else if (pool->blocksInUse == 0)
	{
#if M_DEBUG
		Memfill(fixedBlockHeader, 0xdeaddead, PageAllocationSize);
#endif

		PlatformMemory::PlatformFree(fixedBlockHeader);
		memoryStats.allocatedFixedMemory -= PageAllocationSize;

		// Reset blockInfo
		MemoryBlockInfo* blockInfo = FindExistingMemoryInfo(p);
		Assert(blockInfo);
		DeinitializeMemoryInfo(*blockInfo);

		// Unlink pool from list of pools
		if (pool->prev)
		{
			pool->prev->next = pool->next;
		}
		if (pool->next)
		{
			pool->next->prev = pool->prev;
		}
		if (pool == tableElement.availablePools)
		{
			tableElement.availablePools = pool->next;
		}

		poolManager.ReturnPoolNode(*pool);
	}
}

forceinline void FreeLargeBlock(void* p)
{
	// Get block information
	MemoryBlockInfo* blockInfo = FindExistingMemoryInfo(p);
	Assert(blockInfo);

	PlatformMemory::PlatformFree(p);

	memoryStats.allocatedBigMemory -= blockInfo->allocatedSize;
	memoryStats.usedBigMemory -= blockInfo->allocatedSize;

	DeinitializeMemoryInfo(*blockInfo);
}
}

