// Copyright 2020, Nathan Blane

#pragma once
#include "Heap.h"

class Block
{
public:
	// defines for the heap name length
	static const unsigned int NameLength = 32;
	static const unsigned int NameNumChar = (Heap::NameLength - 1);

public:
	// Public methods
	Block *GetHeapNext();
	Block *GetHeapPrev();
	Block *GetGlobalNext();
	Block *GetGlobalPrev();
	const char *GetFileName();
	uint32 GetLineNum();
	uint32 GetAllocIndex();
	uint64 GetAllocSize();

	Block(const Block&) = delete;
	Block& operator=(const Block&) = delete;

protected:
	Block(const char* fileName, uint32 lineNumber, size_t allocationSize, uint32 allocationIndex);
	~Block();

	// Total Size of this class:
	// 64 Bytes - add appropriate padding

	// Note: need to add Tracking Block links for Heap and Global lists
	//       Several ways to do it... I don't care
	//       No Templates

	// Total Size of this class:
	// 64 Bytes - add appropriate padding

   // Required
	Block* globalNext;
	Block* globalPrev;
	uint64   mAllocSize;
	char           mFileName[Block::NameLength];
	uint32            mLineNum;
	uint32            mAllocIndex;

	Block* heapNext;
	Block* heapPrev;

	Block          *pSecret;   // back pointer to the top of the tracking block
};

