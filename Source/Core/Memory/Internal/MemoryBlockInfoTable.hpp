// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "Debugging/Assertion.hpp"
#include "Internal/MemoryInternalDefinitions.hpp"

namespace Memory::Internal
{
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

}
