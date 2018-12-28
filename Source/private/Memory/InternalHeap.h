#pragma once

#include "Heap.h"

namespace Internal
{
	class InternalHeap : public Heap
	{
	public:
		InternalHeap(const char* heapName, size_t heapSize, Heap* nextHeap, HANDLE heapHandle, Memory* memEngine);
		~InternalHeap();
		InternalHeap(const InternalHeap&) = delete;
		InternalHeap& operator=(const InternalHeap&) = delete;

		void* Malloc(size_t inSize, unsigned int align, char *inName, int lineNum, int allocationIndex);
		void Free(void* memory);

		void Destroy();

		// TODO - Get rid of this interface stuff
		// It makes more sense that internal systems are either friended
		// or internal to the system
		Heap* GetNextHeap() const;
		Heap* GetPrevHeap() const;

		void SetNextHeap(Heap* nextHeap);
		void SetPrevHeap(Heap* prevHeap);

		bool AddressInHeap(const void* data) const;
	};
}