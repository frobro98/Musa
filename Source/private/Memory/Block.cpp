//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#include <stdio.h>
#include <string.h>

#include "Block.h"

Block::Block(char * fileName, int lineNumber, size_t allocationSize, int allocationIndex)
	:mLineNum(lineNumber),
	mAllocIndex(allocationIndex),
	mAllocSize(allocationSize)
{
	pSecret = this;

	// Name initialization
	char* pathStrippedFileName = strrchr(fileName, '\\') + 1;
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

char *Block::GetFileName()
{
	return mFileName;
}

int Block::GetLineNum()
{
	return mLineNum;
}

int Block::GetAllocIndex()
{
	return mAllocIndex;
}

unsigned int Block::GetAllocSize()
{
	return mAllocSize;
}

// ---  End of File ---------------
