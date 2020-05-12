// Copyright 2020, Nathan Blane

#include <new.h>  // for placement new

#include "Heap.h"
#include "MemoryUtilities.h"
#include "Internal/InternalBlock.h"
#include "Internal/InternalMem.h"

using namespace Internal;

void Heap::GetInfo( Info &retInfo)
{
	retInfo = mInfo;
};

#ifdef _DEBUG
Heap *Heap::DebugGetNext() const
{
	return 	heapNext;
};

Heap *Heap::DebugGetPrev() const
{
	return heapPrev;
}
#endif // _DEBUG

Heap::Heap(const char* heapName, size_t heapSize, Heap* nextHeap, HANDLE heapHandle, Memory* memEngine)
	: pMem(memEngine),
	mWinHeapHandle(heapHandle), 
	heapNext(nextHeap),
	heapPrev(nullptr)
{
	if (heapNext != nullptr)
	{
		heapNext->heapPrev = this;
	}

	mInfo.TotalSize = heapSize;
	mInfo.StartAddr = (uint64)this;
	mInfo.EndAddr = mInfo.StartAddr + heapSize;
	
	// Name initialization
	size_t nameLen = strlen(heapName);
	if (nameLen > Heap::NameLength)
	{
		nameLen = Heap::NameNumChar;
	}
	strncpy_s(mInfo.Name, Heap::NameLength, heapName, nameLen);
	mInfo.Name[Heap::NameNumChar] = '\0';
}

Heap::~Heap()
{
}

