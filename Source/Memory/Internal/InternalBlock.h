// Copyright 2020, Nathan Blane

#pragma once

#include "Block.h"

namespace Internal
{
	class InternalBlock : public Block
	{
	public:
		InternalBlock(const char* fileName, uint32 lineNumber, size_t allocationSize, uint32 allocationIndex);
		~InternalBlock();
		InternalBlock(const InternalBlock&) = delete;
		InternalBlock& operator=(const InternalBlock&) = delete;

		void SetGlobalNext(Block* gNext);
		void SetGlobalPrev(Block* gPrev);
		void SetHeapNext(Block* hNext);
		void SetHeapPrev(Block* hPrev);
	};
}