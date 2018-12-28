#include "InternalBlock.h"

namespace Internal
{
	InternalBlock::InternalBlock(char * fileName, int lineNumber, size_t allocationSize, int allocationIndex)
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