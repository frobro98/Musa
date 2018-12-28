#include "InternalHeap.h"

namespace Internal
{
	InternalHeap::InternalHeap(const char * heapName, size_t heapSize, Heap* nextHeap, HANDLE heapHandle, Memory * memEngine)
		: Heap(heapName, heapSize, nextHeap, heapHandle, memEngine)
	{
	}

	InternalHeap::~InternalHeap()
	{

	}
	void * InternalHeap::Malloc(size_t inSize, unsigned int align, char * inName, int lineNum, int allocationIndex)
	{
		return MallocInternal(inSize, align, inName, lineNum, allocationIndex);
	}

	void InternalHeap::Free(void * memory)
	{
		FreeInternal(memory);
	}

	void InternalHeap::Destroy()
	{
		DestroyInternal();
	}

	Heap* InternalHeap::GetNextHeap() const
	{
		return heapNext;
	}

	Heap* InternalHeap::GetPrevHeap() const
	{
		return heapPrev;
	}

	void InternalHeap::SetNextHeap(Heap * nextHeap)
	{
		heapNext = nextHeap;
	}

	void InternalHeap::SetPrevHeap(Heap * prevHeap)
	{
		heapPrev = prevHeap;
	}

	bool InternalHeap::AddressInHeap(const void * data) const
	{
		unsigned int addrData = (unsigned int)data;
		return mInfo.StartAddr < addrData && addrData < mInfo.EndAddr;
	}
}