// Copyright 2020, Nathan Blane

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

		void* Malloc(uint64 inSize, uint32 align, const char *inName, uint32 lineNum, uint32 allocationIndex);
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