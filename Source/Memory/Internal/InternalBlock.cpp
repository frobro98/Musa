// Copyright 2020, Nathan Blane

#include "InternalBlock.h"

namespace Internal
{
	InternalBlock::InternalBlock(const char * fileName, uint32 lineNumber, size_t allocationSize, uint32 allocationIndex)
		: Block(fileName, lineNumber, allocationSize, allocationIndex)
	{
	}

	InternalBlock::~InternalBlock()
	{
	}

	void InternalBlock::SetGlobalNext(Block * gNext)
	{
		globalNext = gNext;
	}

	void InternalBlock::SetGlobalPrev(Block* gPrev)
	{
		globalPrev = gPrev;
	}

	void InternalBlock::SetHeapNext(Block* hNext)
	{
		heapNext = hNext;
	}

	void InternalBlock::SetHeapPrev(Block* hPrev)
	{
		heapPrev = hPrev;
	}
}