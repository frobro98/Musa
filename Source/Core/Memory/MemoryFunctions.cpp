// Copyright 2020, Nathan Blane

#include "MemoryFunctions.hpp"
#include "Platform/PlatformMemory.hpp"
#include "Debugging/Assertion.hpp"
// TODO - IsPowerOf2 shouldn't be in this header...
#include "Utilities/MemoryUtilities.hpp"
#include "Utilities/Array.hpp"

#include <malloc.h>

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

enum class BlockType : uint32
{
	FixedSmallBlock,
	DynamicBlock
};

enum OwnedBlockFlag : uint32
{
	NoBlock = 0xdead,
	YesBlock = 0xdada
};

constexpr size_t TotalSmallFixedTableSizes = 64;

constexpr size_t SmallFixedTableSizes[] = {
	16, 32, 48, 64, 80, 96, 112, 128, 160, 192, 
	224, 256, 336, 448, 496, 512, 544, 576, 608, 640,
	672, 704, 736, 768, 800, 864, 928, 992, 1024, 1152,
	1280, 1408, 1536, 1664, 1792, 1920, 2048, 2304, 2560, 2816,
	3072, 3328, 3584, 3840, 4096, 4352, 4608, 5120, 5632, 6144,
	6656, 7680, 8704, 9728, 10752, 11776, 13824, 15872, 17920, 19968,
	22016, 24064, 28160, 32768
};

constexpr size_t ReversedSmallFixedTableSizes[] = {
	32768, 28160, 24064, 22016, 19968, 17920, 15872, 13824, 11776, 10752,
	9728, 8704, 7680, 6656, 6144, 5632, 5120, 4608, 4352, 4096,
	3840, 3584, 3328, 3072, 2816, 2560, 2304, 2048, 1920, 1792,
	1664, 1536, 1408, 1280, 1152, 1024, 992, 928, 864, 800,
	768, 736, 704, 672, 640, 608, 576, 544, 512, 496,
	448, 336, 256, 224, 192, 160, 128, 112, 96, 80,
	64, 48, 32, 16
};

static_assert(ArraySize(SmallFixedTableSizes) == TotalSmallFixedTableSizes);
static_assert(ArraySize(ReversedSmallFixedTableSizes) == TotalSmallFixedTableSizes);

// 32 bytes
struct MemoryBlockInfo
{
	size_t allocatedSize; // Size of associated block, alignment taken into consideration
	OwnedBlockFlag flag;
	BlockType type; // Useful to know what block of memory I'm dealing with
	uint64 pad[2];
};

static_assert(sizeof(MemoryBlockInfo) == 32);

void AssertValidMemoryBlock(const MemoryBlockInfo& block)
{
	Assert(block.flag != NoBlock);
}



namespace Memory
{
static bool isInitialized = false;
void InitializeMemory()
{
	NOT_USED PlatformMemoryInfo memInfo = GetPlatformMemoryInfo();

	// Use memory info to generate however many allocation meta data buckets there are, forever

	// Initialize small alloc table, currently using 128 size

	isInitialized = true;
}
void* Memory::Malloc(size_t size, size_t alignment)
{
	Assert(isInitialized);
	Assert(IsPowerOf2(alignment));
	// Check size to see if it's going to end up in the fixed block allocation pools

	return _aligned_malloc(size, alignment);
}

void* Memory::Realloc(void* ptr, size_t size, size_t alignment)
{
	Assert(isInitialized);
	Assert(IsPowerOf2(alignment));
	// Checks the actual size of the memory within the ptrs current allocation
	// If it can fit the new size within the current block, it just returns that ptr
	// else it uses Malloc and Free to get a new allocation
	return _aligned_realloc(ptr, size, alignment);
}

void Memory::Free(void* p)
{
	Assert(isInitialized);
	// Main issue here is verifying that the pointer that comes back is in memory that this system has allocated

	// The way I think would work is having a fixed-size collection of meta data where each 

	_aligned_free(p);
}

}

// Initialize Memory Core first thing
namespace
{
#pragma init_seg(user)
struct InitMemoryCore
{
	InitMemoryCore()
	{
		Memory::InitializeMemory();
	}
};

InitMemoryCore initObject;
}
