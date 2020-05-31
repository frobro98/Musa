// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "Memory/MemoryCore.hpp"
#include "Internal/MemoryFixedBlock.hpp"
#include "Internal/MemoryBlockInfoTable.hpp"
#include "Platform/PlatformMemory.hpp"
#include "Debugging/DebugOutput.hpp"

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
	blockInfoHashTable = (MemoryBlockInfoBucket*)Memory::PlatformAlloc(MemoryBucketCount * sizeof(MemoryBlockInfoBucket));

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
	NOT_USED Memory::PlatformMemoryInfo memInfo = Memory::GetPlatformMemoryInfo();

	// Use memory info to generate however many allocation meta data buckets there are, forever

	InitializeFixedBlockTable();

	InitializeMemoryInfoTable();

	isInitialized = true;
}

static void DeinitializeMemory()
{
	Assert(isInitialized);

	Debug::Printf("Big memory allocations: {}\n", memoryStats.allocatedBigMemory);
	Debug::Printf("Fixed memory allocations: {}\n", memoryStats.allocatedFixedMemory);

	isInitialized = false;
}

//////////////////////////////////////////////////////////////////////////
// Memory Block Info Accessors
//////////////////////////////////////////////////////////////////////////
static forceinline MemoryBlockInfo& InitializeOrFindMemoryInfo(void* p, BlockType blockType)
{
	Assert(p);
	uptr addr = reinterpret_cast<uptr>(p);
	uptr preprocessedAddr = addr >> BitsInPageAllocation;
	u64 blockIndex = preprocessedAddr & BlockInfoMask;
	u64 memoryBucketIndex = (preprocessedAddr >> BitsOfBlockInfoMask) & MemoryBucketMask;

	// TODO - Make this parallel arrays of some kind
	MemoryBlockInfoBucket& bucket = blockInfoHashTable[memoryBucketIndex];
	if (bucket.blockInfo == nullptr)
	{
		bucket.blockInfo = (MemoryBlockInfo*)Memory::PlatformAlloc(PageAllocationSize);
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

	FixedBlockTableElement& tableElement = fixedBlockTable[tableIndex];
	if (tableElement.activeFront == nullptr)
	{
		// Create pool and link it to front and back
		FreedBlock* newBlock = AllocateNewPoolFor(tableElement, tableIndex);
		memoryStats.allocatedFixedMemory += PageAllocationSize;

		MemoryBlockInfo& blockInfo = InitializeOrFindMemoryInfo(newBlock, BlockType::FixedSmallBlock);
		blockInfo.allocatedSize = tableElement.fixedBlockSize;
	}

	// Allocate block from fixed block pool
	void* p = AllocateFromFreedBlock(*tableElement.activeFront);
	if (tableElement.activeFront->numFreeBlocks == 0)
	{
		tableElement.activeFront = tableElement.activeFront->nextBlock;
		--tableElement.totalFreeBlockCount;
	}
	memoryStats.usedFixedMemory += tableElement.fixedBlockSize;
	return p;
}

forceinline void* MallocLargeBlock(size_t size, size_t alignment)
{
	size_t alignedSize = Align(size, alignment);
	// Do big boi allocation from OS
	void* ret = PlatformAlloc(alignedSize);
	MemoryBlockInfo& blockInfo = InitializeOrFindMemoryInfo(ret, BlockType::FixedSmallBlock);
	blockInfo.allocatedSize = alignedSize;

	memoryStats.allocatedBigMemory += alignedSize;
	memoryStats.usedBigMemory += alignedSize;

	return ret;
}

//////////////////////////////////////////////////////////////////////////
// Free Functions
//////////////////////////////////////////////////////////////////////////
forceinline void FreeFixedBlock(void* p)
{
	// We know now that this is a fixed block allocation. We need to get back to the main "FreedBlock" header
	FreedBlock* fixedBlockHeader = GetFixedBlockHeader(p);
	Assert(fixedBlockHeader->headerID == FreedBlock::BlockTag);
	u8 tableIndex = FixedSizeToTableIndex(fixedBlockHeader->blockSize);
	FixedBlockTableElement& tableElement = fixedBlockTable[tableIndex];

	FreedBlock* freeBlock = (FreedBlock*)p;
	freeBlock->blockSize = fixedBlockHeader->blockSize;
	freeBlock->headerID = FreedBlock::BlockTag;
	freeBlock->numFreeBlocks = 1;
	freeBlock->nextBlock = nullptr;

	tableElement.activeBack->nextBlock = freeBlock;
	tableElement.activeBack = freeBlock;
	++tableElement.totalFreeBlockCount;

	memoryStats.usedFixedMemory -= tableElement.fixedBlockSize;

	// NOTE - Don't need to get the Memory Block Info header here
}

forceinline void FreeLargeBlock(void* p)
{
	// Get block information
	MemoryBlockInfo* blockInfo = FindExistingMemoryInfo(p);
	Assert(blockInfo);

	PlatformFree(p);

	memoryStats.allocatedBigMemory -= blockInfo->allocatedSize;
	memoryStats.usedBigMemory -= blockInfo->allocatedSize;

	DeinitializeMemoryInfo(*blockInfo);
}
}

