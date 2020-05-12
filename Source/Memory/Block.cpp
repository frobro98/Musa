// Copyright 2020, Nathan Blane

#include <stdio.h>
#include <string.h>

#include "Block.h"

Block::Block(const char* fileName, uint32 lineNumber, size_t allocationSize, uint32 allocationIndex)
	: mAllocSize(allocationSize),
	mLineNum(lineNumber),
	mAllocIndex(allocationIndex),
	pSecret(nullptr)
{
	pSecret = this;

	// Name initialization
	const char* pathStrippedFileName = strrchr(fileName, '\\') + 1;
	size_t nameLen = strlen(pathStrippedFileName);
	if (nameLen > Block::NameLength)
	{
		nameLen = Block::NameNumChar;
	}
	strncpy_s(mFileName, Block::NameLength, pathStrippedFileName, nameLen);
	mFileName[Block::NameNumChar] = '\0';
}

Block::~Block()
{
}

Block *Block::GetHeapNext()
{
	return heapNext;
}

Block *Block::GetHeapPrev()
{
	return heapPrev;
}

Block *Block::GetGlobalNext()
{
	return globalNext;
}

Block *Block::GetGlobalPrev()
{
	return globalPrev;
}

const char *Block::GetFileName()
{
	return mFileName;
}

uint32 Block::GetLineNum()
{
	return mLineNum;
}

uint32 Block::GetAllocIndex()
{
	return mAllocIndex;
}

uint64 Block::GetAllocSize()
{
	return mAllocSize;
}

// ---  End of File ---------------
