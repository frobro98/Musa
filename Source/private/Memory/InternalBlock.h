#pragma once

#include "Block.h"

namespace Internal
{
	class InternalBlock : public Block
	{
	public:
		InternalBlock(char* fileName, int lineNumber, size_t allocationSize, int allocationIndex);
		~InternalBlock();
		InternalBlock(const InternalBlock&) = delete;
		InternalBlock& operator=(const InternalBlock&) = delete;

		void SetGlobalNext(Block* gNext);
		void SetGlobalPrev(Block* gPrev);
		void SetHeapNext(Block* hNext);
		void SetHeapPrev(Block* hPrev);
	};
}