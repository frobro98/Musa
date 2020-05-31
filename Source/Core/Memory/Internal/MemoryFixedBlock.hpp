// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "Platform/PlatformMemory.hpp"
#include "Memory/MemoryDefinitions.hpp"
#include "Internal/MemoryBlockInfoTable.hpp"
#include "Utilities/Array.hpp"

namespace Memory::Internal
{
// Header of the entire page of fixed block memory
struct FreedBlock
{
	static constexpr u8 BlockTag = 0xfe;
	static forceinline bool ShouldAdjustForHeader(u16 numFreeBlocks, u16 blockSize)
	{
		return (numFreeBlocks * blockSize + AllocationDefaultAlignment) > PageAllocationSize;
	}

	FreedBlock(u16 blockSize_, u8 tableIndex_)
		: tableIndex(tableIndex_),
		blockSize(blockSize_)
	{
		numFreeBlocks = (u16)(PageAllocationSize / blockSize);
		if (ShouldAdjustForHeader(numFreeBlocks, blockSize))
		{
			--numFreeBlocks;
		}
	}

	// This block is a pointer to the next block outside of the contiguous memory within this block
	FreedBlock* nextBlock = nullptr;
	u8 tableIndex;
	u8 headerID = BlockTag;
	u16 blockSize;
	u16 numFreeBlocks;
};

struct FixedBlockPool
{
	FreedBlock* blocksWithinPool;
	u16 blocksInUse = 0;
};

struct FixedBlockTableElement
{
	FreedBlock* activeFront = nullptr;
	FreedBlock* activeBack = nullptr;
	size_t fixedBlockSize = 0;
	u32 totalFreeBlockCount = 0;
};

constexpr u16 SmallFixedTableSizes[] = {
	16, 32, 48, 64, 80, 96, 112, 128, 160, 192,
	224, 256, 336, 448, 496, 512, 544, 576, 608, 640,
	672, 704, 736, 768, 800, 864, 928, 992, 1024, 1152,
	1280, 1408, 1536, 1664, 1792, 1920, 2048, 2304, 2560, 2816,
	3072, 3328, 3584, 3840, 4096, 4352, 4608, 5120, 5632, 6144,
	6656, 7680, 8704, 9728, 10752, 11776, 13824, 15872, 17920, 19968,
	22016, 24064, 28160, MaxFixedTableSize
};
static_assert(ArraySize(SmallFixedTableSizes) == TotalSmallFixedTableSizes);

// Cache coherency
constexpr u16 ReversedSmallFixedTableSizes[] = {
	MaxFixedTableSize, 28160, 24064, 22016, 19968, 17920, 15872, 13824, 11776, 10752,
	9728, 8704, 7680, 6656, 6144, 5632, 5120, 4608, 4352, 4096,
	3840, 3584, 3328, 3072, 2816, 2560, 2304, 2048, 1920, 1792,
	1664, 1536, 1408, 1280, 1152, 1024, 992, 928, 864, 800,
	768, 736, 704, 672, 640, 608, 576, 544, 512, 496,
	448, 336, 256, 224, 192, 160, 128, 112, 96, 80,
	64, 48, 32, 16
};
static_assert(ArraySize(ReversedSmallFixedTableSizes) == TotalSmallFixedTableSizes);

static_assert(sizeof(FreedBlock) <= SmallFixedTableSizes[0],
	"The freed block header needs to be able to fit inside of 16 bytes");

static forceinline bool ShouldUseFixedBlocks(size_t size, size_t alignment)
{
	return size <= MaxFixedTableSize && alignment <= AllocationDefaultAlignment;
}

static forceinline u16 TableIndexToFixedSize(u8 tableIndex)
{
	return ReversedSmallFixedTableSizes[TotalSmallFixedTableSizes - tableIndex - 1];
}

static forceinline FreedBlock* AllocateNewPoolFor(FixedBlockTableElement& tableElement, u8 tableIndex)
{
	void* alloc = Memory::PlatformAlloc(PageAllocationSize);
	// TODO - Check for out of memory issues
	FreedBlock* block = new(alloc) FreedBlock((u16)tableElement.fixedBlockSize, tableIndex);
	tableElement.activeFront = block;
	tableElement.activeBack = block;
	tableElement.totalFreeBlockCount = 1;

	return block;
}

static forceinline void* AllocateFromFreedBlock(FreedBlock& block)
{
	--block.numFreeBlocks;
	if (IsAllocationFromOS(&block))
	{
		return (u8*)&block + PageAllocationSize - (block.numFreeBlocks + 1) * block.blockSize;
	}
	else
	{
		return &block;
	}
	//--block.numFreeBlocks;
}

static forceinline FreedBlock* GetFixedBlockHeader(void* p)
{
	return (FreedBlock*)((uptr)p & OSAllocationBitMask);
}
}
