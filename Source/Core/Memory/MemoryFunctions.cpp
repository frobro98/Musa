// Copyright 2020, Nathan Blane

#include "MemoryFunctions.hpp"
#include "Containers/StaticArray.hpp"
#include "Platform/PlatformMemory.hpp"
#include "Debugging/Assertion.hpp"
#include "Debugging/DebugOutput.hpp"
// TODO - IsPowerOf2 shouldn't be in this header...
#include "Utilities/MemoryUtilities.hpp"
#include "Utilities/Array.hpp"
#include "Utilities/BitUtilities.hpp"
#include "MemoryCore.hpp"

// "Small" allocations
// 16 bytes - 32kb allocations fit in here
// Cached size -> index into table conversion
// Small allocations probably will take up the most allocations (not the most memory)
//
// Large allocations coming directly from the OS
// There will be some kind of caching solution that keeps a specific amount of memory
// around from these large allocations
//
// Blocks of memory need to have some sort of header to them, but without actually having
// the memory contain the header. Block info would describe the actual size of the block,
// be associated with a block of allocated memory, and live in some other structure that 
// uses the ptr address as a unique id to access this info. If I use a page (64Kb) to 
//
// So how to split up the address (unique id) so that they fall into a set amount of elements of block info?
// By shifting address over 16 (2 ^ 16 is 65536 or page size), we get rid of the 0s that are there because of alignment
// and get a more workable number. The bits at the bottom of the address vary a fair amount between allocations vs the upper bits,
// so this could be the table elements within the hash table
//
// This algorithm uses the important parts of the address, as well as the entire memory address spectrum
// This is currently on my system, which has 16Gb memory (really a little less, but it uses 16Gb exactly)
// Max buckets for this amount of memory is 128 and max pools per 64k is 2048 (pool is 32 bytes).
// You get these numbers by using the total amount of 64Kb allocations a system can essentially have
//
// My system (16Gb) can have have 262,144 allocations of 64kb (17,179,849,184 / 65536).
// If I were to allocate 64Kb per hash table element, I could fit 2048 32 byte headers within that memory.
// Because of this, if I divide total possible page allocations by number of headers per element (262,144 / 2048)
// I get the amount of table elements that are needed, which is 128
//
// CURRENTLY, THE THEORY BELOW DOESN'T WORK... NEED TO THINK ABOUT THIS MORE. USING 1MB COULD OFFER BETTER RESULTS DOWN THE ROAD...
// MIGHT BE ABLE TO MAP THE INDEX RECEIVED BY THE PTR TO SOME OTHER STRUCTURE THAT DOESN'T TAKE UP A WHOLE MEGABYTE TO MAKE THIS WORK...
// If I were to up my size and allocate 1Mb (1,048,576), I would be able to do that 16,384 times
// Keeping my page allocation, and the header count of 2048, I would do 16,384 / 2048, which results in 8 table elements
//
// For not power of 2 memory spaces, needs to essentially go to the nearest bit bit above the total memory space

// TODO - Research Virtual memory and understand how it works and how to map a lot of virtual memory space to 
// physical memory and make it worth while

// TODO - This same thing is defined in the header file...
constexpr u32 AllocationDefaultAlignment = 16;
constexpr u32 BitsForDefaultAlignment = 4;

constexpr size_t PageAllocationSize = KilobytesAsBytes(64);
constexpr size_t OSAllocationAlignment = KilobytesAsBytes(64);
constexpr u64 OSAllocationBitMask = 0xffffffffffff0000;
constexpr u64 BitsInPageAllocation = 16;

constexpr size_t TotalSmallFixedTableSizes = 64;
// Used for some sizes so that the header can fit into the page
constexpr size_t FixedHeaderAdjust = 16;
constexpr size_t MaxFixedTableSize = (PageAllocationSize / 2) - FixedHeaderAdjust; // Adjust to allow for FreedBlock header

enum class BlockType : u32
{
	Untyped,
	FixedSmallBlock,
	DynamicBlock
};

enum class OwnedBlockFlag : u32
{
	NoBlock = 0xdead,
	YesBlock = 0xdada
};

// 32 bytes
struct MemoryBlockInfo
{
	size_t allocatedSize = 0; // Size of associated block, alignment taken into consideration
	OwnedBlockFlag flag = OwnedBlockFlag::NoBlock;
	BlockType type = BlockType::Untyped; // Useful to know what block of memory I'm dealing with
	u64 pad[2] = {};
};
static_assert(sizeof(MemoryBlockInfo) == 32);

// TODO - Investigate whether my theory about total memory allocations is true. 
// I think it is, if we weren't in the virtual memory space...
// If this thought is correct, then there will be collisions for buckets
struct MemoryBlockInfoBucket
{
	MemoryBlockInfo* blockInfo = nullptr;
};

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

constexpr u32 MemoryBucketCount = 128; // This is the upper bits of a ptr address that isn't being used for the pools
constexpr u64 MemoryBucketMask = MemoryBucketCount - 1;
constexpr u16 FixedSizeCacheCount = 2048;
constexpr u16 BlockInfosPerBucket = 2048;
constexpr u64 BlockInfoMask = BlockInfosPerBucket - 1;
constexpr u32 BitsOfMemoryBucketMask = 7;
constexpr u32 BitsOfBlockInfoMask = 11;
static u8 fixedSizeToIndexCache[2048];

// Initialize Memory Core first thing
#pragma warning(disable:4075)
#pragma init_seg(".CRT$XCT")
namespace Memory
{
static void InitializeMemory();
static void DeinitializeMemory();
}
static FixedBlockTableElement fixedBlockTable[TotalSmallFixedTableSizes];
static MemoryBlockInfoBucket* blockInfoHashTable = nullptr;

struct MemoryCoreController
{
	MemoryCoreController()
	{
		Memory::InitializeMemory();
	}
	~MemoryCoreController()
	{
		Memory::DeinitializeMemory();
	}
};

MemoryCoreController initObject;
#pragma warning(default:4075)

struct MemoryStats
{
	size_t allocatedFixedMemory = 0;
	size_t allocatedBigMemory = 0;
	size_t usedFixedMemory = 0;
	size_t usedBigMemory = 0;
};
static MemoryStats memoryStats;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Memory table
//////////////////////////////////////////////////////////////////////////
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

static forceinline MemoryBlockInfo* FindExistingMemoryInfo(uptr p)
{
	Assert(p > 0);

	uptr preprocessedAddr = p >> BitsInPageAllocation;
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

static forceinline bool ShouldUseFixedBlocks(size_t size, size_t alignment)
{
	return size <= MaxFixedTableSize && alignment <= AllocationDefaultAlignment;
}

static forceinline u8 FixedSizeToTableIndex(size_t alignedSize)
{
	// Bump up size to next 16 byte aligned size. Index 0 and 1 are associated with 16, so
	// this needs to ignore 0 index, which means bump up size to next alignment boundary
	size_t fixedSizeCacheIndex = (alignedSize + AllocationDefaultAlignment - 1) >> BitsForDefaultAlignment;
	Assert(fixedSizeCacheIndex > 0 && fixedSizeCacheIndex <= (MaxFixedTableSize >> BitsForDefaultAlignment));
	u8 tableIndex = fixedSizeToIndexCache[fixedSizeCacheIndex];
	return tableIndex;
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
	// TODO - Need to register new block with memory info structure!

	memoryStats.allocatedFixedMemory += PageAllocationSize;

	MemoryBlockInfo& blockInfo = InitializeOrFindMemoryInfo(alloc, BlockType::FixedSmallBlock);
	blockInfo.allocatedSize = tableElement.fixedBlockSize;

	return block;
}

static forceinline bool IsAllocationFromOS(void* ptr)
{
	return IsAligned(ptr, OSAllocationAlignment);
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

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// TODO - Look into using a Best-Fit RedBlack tree to store medium allocations (Hacky talked about this idea in GDC talk)
// Might not be needed, but it might be good to look at stats of allocations

namespace Memory
{

static bool isInitialized = false;

static void InitializeMemory()
{
	Assert(!Memory::isInitialized);
	NOT_USED Memory::PlatformMemoryInfo memInfo = Memory::GetPlatformMemoryInfo();

	// Use memory info to generate however many allocation meta data buckets there are, forever

	InitializeFixedBlockTable();
	
	InitializeMemoryInfoTable();

	Memory::isInitialized = true;
}

static void DeinitializeMemory()
{
	Assert(isInitialized);

	Debug::Printf("Big memory allocations: {}\n", memoryStats.allocatedBigMemory);
	Debug::Printf("Fixed memory allocations: {}\n", memoryStats.allocatedFixedMemory);

	isInitialized = false;
}

void* Memory::Malloc(size_t size, size_t alignment)
{
	Assert(isInitialized);
	Assert(IsPowerOf2(alignment));

	if (ShouldUseFixedBlocks(size, alignment))
	{
		// Do fixed block allocation
		u8 tableIndex = FixedSizeToTableIndex(size);

		FixedBlockTableElement& tableElement = fixedBlockTable[tableIndex];
		if (tableElement.activeFront == nullptr)
		{
			// Create pool and link it to front and back
			AllocateNewPoolFor(tableElement, tableIndex);
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
	else
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
}


// Checks the actual size of the memory within the ptrs current allocation
// If it can fit the new size within the current block, it just returns that ptr
// else it uses Malloc and Free to get a new allocation
void* Memory::Realloc(void* ptr, size_t size, size_t alignment)
{
	Assert(isInitialized);
	Assert(IsPowerOf2(alignment));

	// TODO - Lots of branching. See if it can be less
	if (size > 0)
	{
		// Reallocing a fixed block
		if (!IsAllocationFromOS(ptr))
		{
			FreedBlock* fixedHeader = GetFixedBlockHeader(ptr);
			Assert(fixedHeader->headerID == FreedBlock::BlockTag);

			// Need to check if the size can fit within this fixed block, or if it needs
			// to find a new block
			u8 tableIndex = fixedHeader->tableIndex;
			u16 fixedSize = fixedHeader->blockSize;
			// Checks to see if block needs to be actually realloced
			if (size > fixedHeader->blockSize || alignment > AllocationDefaultAlignment || // Greater than this
				(tableIndex != 0 && size <= TableIndexToFixedSize(tableIndex - 1))) // Check if the allocation can fit into the previous size element
			{
				void* ret = Malloc(size, alignment);
				Memcpy(ret, ptr, fixedSize);
				Free(ptr);
				return ret;
			}
			else
			{
				return ptr;
			}
		}
		// Not null, but not a fixed block allocation
		else if (ptr)
		{
			MemoryBlockInfo* blockInfo = FindExistingMemoryInfo(reinterpret_cast<uptr>(ptr));
			Assert(blockInfo);
			size_t allocatedSize = blockInfo->allocatedSize;
			// Test to see if it can use fixed block, if allocation cann't fit into the current allocation
			
			bool useFixed = ShouldUseFixedBlocks(size, alignment);
			if (useFixed || size > allocatedSize)
			{
				void* ret = nullptr;
				if (useFixed)
				{
					// Same code that is used in Malloc to allocate fixed block sizes
					// Do fixed block allocation
					u8 tableIndex = FixedSizeToTableIndex(size);

					FixedBlockTableElement& tableElement = fixedBlockTable[tableIndex];
					if (tableElement.activeFront == nullptr)
					{
						// Create pool and link it to front and back
						AllocateNewPoolFor(tableElement, tableIndex);
					}

					// Allocate block from fixed block pool
					ret = AllocateFromFreedBlock(*tableElement.activeFront);
					if (tableElement.activeFront->numFreeBlocks == 0)
					{
						tableElement.activeFront = tableElement.activeFront->nextBlock;
						--tableElement.totalFreeBlockCount;
					}

					memoryStats.allocatedFixedMemory += tableElement.fixedBlockSize;
				}
				else
				{
					// Same code that is used in Malloc to allocate larger allocations
					size_t alignedSize = Align(size, alignment);
					ret = PlatformAlloc(alignedSize);
					MemoryBlockInfo& largeBlockInfo = InitializeOrFindMemoryInfo(ret, BlockType::FixedSmallBlock);
					largeBlockInfo.allocatedSize = alignedSize;

					memoryStats.allocatedBigMemory += (alignedSize - allocatedSize);
					memoryStats.usedBigMemory += (alignedSize - allocatedSize);
				}

				Memcpy(ret, ptr, allocatedSize);
				Free(ptr);
				return ret;
			}

			return ptr;
		}
		else
		{
			// if null, just call malloc. Nothing to realloc
			return Malloc(size, alignment);
		}
	}

	// Size is 0, so we free ptr
	Free(ptr);
	return nullptr;
}

void Memory::Free(void* p)
{
	Assert(isInitialized);
	// Main issue here is verifying that the pointer that comes back is in memory that this system has allocated
	
	// The way I think would work is having a fixed-size collection of meta data where each 

	if (!IsAllocationFromOS(p))
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

// 		MemoryBlockInfo* blockInfo = FindExistingMemoryInfo(reinterpret_cast<uptr>(p));
// 		Assert(blockInfo);
// 		DeinitializeMemoryInfo(*blockInfo);
	}
	else if(p)
	{
		// p should be non-null so we end up not doing anything for Free on a nullptr
		
		// Get block information
		MemoryBlockInfo* blockInfo = FindExistingMemoryInfo(reinterpret_cast<uptr>(p));
		Assert(blockInfo);

		PlatformFree(p);

		memoryStats.allocatedBigMemory -= blockInfo->allocatedSize;
		memoryStats.usedBigMemory -= blockInfo->allocatedSize;

		DeinitializeMemoryInfo(*blockInfo);
	}
}

}


