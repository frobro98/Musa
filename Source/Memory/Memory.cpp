// Copyright 2020, Nathan Blane

#include <stdio.h>

#include "Internal/InternalMem.h"
#include "DynamicBlockHeap.h"
#include "FixedBlockHeap.h"
#include "Internal/InternalBlock.h"
#include "MemoryUtilities.h"
#include "Assertion.h"

// Allowing use of Internal
using namespace Internal;

// Internal Alignment helper
unsigned int GetAlignment(Memory::Align alignment)
{
	switch (alignment)
	{
		case Memory::Byte_4:
		{
			return 4;
		}
		case Memory::Byte_8:
		{
			return 8;
		}
		case Memory::Byte_16:
		{
			return 16;
		}
		case Memory::Byte_32:
		{
			return 32;
		}
		case Memory::Byte_64:
		{
			return 64;
		}
		case Memory::Byte_128:
		{
			return 128;
		}
		case Memory::Byte_256:
		{
			return 256;
		}
		default:
		{
			// Don't know why this happened
			Assert(false);
		}
	}
	return 0;
}

// Create the heap.
Memory::Code Memory::HeapCreate(Heap *&newHeap, unsigned int heapSize, const char * const Name)
{
	Memory* mem = Instance();
	if (!mem->mInitialized)
	{
		return ERROR_Mem_Not_Initialized;
	}

	HANDLE heapHandle = ::HeapCreate(0, heapSize, 0);
	if (heapHandle == INVALID_HANDLE_VALUE)
	{
		return ERROR_Heap_Create;
	}

	void* heapRaw = ::HeapAlloc(heapHandle, 0, sizeof(DynamicBlockHeap));
	if (heapRaw == nullptr)
	{
		return ERROR_Heap_Create;
	}

	DynamicBlockHeap* heap = new(heapRaw) DynamicBlockHeap(Name, heapSize, mem->pHeapHead, heapHandle, mem);
	mem->pHeapHead = heap;

	++mem->mInfo.CurrHeapCount;
	if (mem->mInfo.CurrHeapCount > mem->mInfo.PeakHeapCount)
	{
		++mem->mInfo.PeakHeapCount;
	}

	newHeap = heap;

	return OK;
}

Memory::Code Memory::HeapCreate(Heap *&newHeap, uint32 numBlocks, uint32 sizePerBlock, const char * const Name)
{
	Memory* mem = Instance();
	if (!mem->mInitialized)
	{
		return ERROR_Mem_Not_Initialized;
	}

	uint32 heapSize = (uint32)sizeof(FixedBlockHeap) + (numBlocks * sizePerBlock);

	HANDLE heapHandle = ::HeapCreate(0, heapSize, 0);
	if (heapHandle == 0)
	{
		return ERROR_Heap_Create;
	}

	void* heapRaw = ::HeapAlloc(heapHandle, 0, sizeof(FixedBlockHeap));
	if (heapRaw == nullptr)
	{
		return ERROR_Heap_Create;
	}

	FixedBlockHeap* heap = new(heapRaw) FixedBlockHeap(mem->pHeapHead, Name, numBlocks, sizePerBlock, heapHandle, mem);
	mem->pHeapHead = heap;

	++mem->mInfo.CurrHeapCount;
	if (mem->mInfo.CurrHeapCount > mem->mInfo.PeakHeapCount)
	{
		++mem->mInfo.PeakHeapCount;
	}

	newHeap = heap;

	return OK;
}

void* Memory::Malloc(uint64 inSize, Heap *heap, Memory::Align align, const char *inName, uint32 lineNum)
{
	Memory* mem = Instance();
	Assert(mem->mInitialized);
	Assert(heap != nullptr);

	uint32 alignment = GetAlignment(align);
	
	mem->mInfo.CurrBytesUsed += inSize;
	++mem->mInfo.CurrNumAlloc;
	++mem->mInfo.CurrAllocIndex;

	InternalHeap* memHeap = static_cast<InternalHeap*>(heap);
	void* memPtr = memHeap->Malloc(inSize, alignment, inName, lineNum, mem->mInfo.CurrAllocIndex);
	// TODO - this neeeeeeds to be moved somewhere else...
	InternalBlock* block = reinterpret_cast<InternalBlock*>(GetSecretPointer(memPtr));
	block->SetGlobalNext(mem->pGlobalHead);
	if (mem->pGlobalHead != nullptr)
	{
		(static_cast<InternalBlock*>(mem->pGlobalHead))->SetGlobalPrev(block);
	}
	mem->pGlobalHead = block;

	if (mem->mInfo.CurrBytesUsed > mem->mInfo.PeakBytesUsed)
	{
		mem->mInfo.PeakBytesUsed += inSize;
	}
	if (mem->mInfo.CurrNumAlloc > mem->mInfo.PeakNumAlloc)
	{
		++mem->mInfo.PeakNumAlloc;
	}

	return memPtr;
}

void Memory::Free(void * data, Heap* heap)
{
	Memory* mem = Instance();
	Block* block = reinterpret_cast<Block*>(GetSecretPointer(data));
	mem->mInfo.CurrBytesUsed -= block->GetAllocSize();
	--mem->mInfo.CurrNumAlloc;

	InternalBlock* globalNext = static_cast<InternalBlock*>(block->GetGlobalNext());
	InternalBlock* globalPrev = static_cast<InternalBlock*>(block->GetGlobalPrev());
	if (globalNext != nullptr)
	{
		globalNext->SetGlobalPrev(globalPrev);
	}
	if (globalPrev != nullptr)
	{
		globalPrev->SetGlobalNext(globalNext);
	}

	if (mem->pGlobalHead == block)
	{
		mem->pGlobalHead = globalNext;
	}

	((InternalHeap*)heap)->Free(data);
}

// Initialize the memory system  (Public method)
Memory::Code Memory::Initialize()
{
	Memory::Code result = OK;
	Memory* memSystem = Instance();
	if (memSystem == nullptr)
	{
		result = ERROR_Mem_Not_Initialized;
	}
	else
	{
		memSystem->mInitialized = true;
		memSystem->mInfo.CurrAllocIndex = 0;
		memSystem->mInfo.PeakHeapCount = 0;
		memSystem->mInfo.CurrHeapCount = 0;
		memSystem->mInfo.PeakNumAlloc = 0;
		memSystem->mInfo.CurrNumAlloc = 0;
		memSystem->mInfo.PeakBytesUsed = 0;
		memSystem->mInfo.CurrBytesUsed = 0;

		memSystem->pHeapHead = nullptr;
		memSystem->pGlobalHead = nullptr;
		// Zero out padding
		Memset(memSystem->pad, 0, ArraySize(memSystem->pad));

#ifdef _DEBUG
		memSystem->CreateDebugHeap();
#endif
	}

	return result;
}

Memory::Code Memory::Destroy(void)
{
	Memory* mem = Instance();
	if (!mem->mInitialized)
	{
		return ERROR_Mem_Not_Initialized;
	}

	mem->mInitialized = false;
	return OK;
}

Memory::Code Memory::GetInfo(Memory::Info &retInfo)
{
	Memory::Code result = OK;
	Memory* mem = Instance();
	if (!mem->mInitialized)
	{
		result = ERROR_Mem_Not_Initialized;
	}
	else
	{
		retInfo = mem->mInfo;
	}
	return result;
}

Memory* Memory::Instance()
{
	static InternalMem memInstance;
	return &memInstance;
}

#ifdef _DEBUG
Heap *Memory::DebugGetHeapHead()
{
	return Instance()->pHeapHead;
}

Block *Memory::DebugGetGlobalTrackingHead()
{
	return Instance()->pGlobalHead;
}
#endif // _DEBUG

#ifdef _DEBUG
void Memory::CreateDebugHeap()
{
	const int numBlocks = 10000;
	unsigned int heapSize = sizeof(Block) * 10000 + (2*1024);
	HANDLE heapHandle = ::HeapCreate(0, heapSize, 0);
	Assert(heapHandle != INVALID_HANDLE_VALUE);
	// TODO - finish refactor of this debug heap idea
}
#endif

void Memory::RemoveHeap(Heap* inHeap)
{
	InternalHeap* heap = static_cast<InternalHeap*>(inHeap);

	if (heap->GetPrevHeap() != nullptr)
	{
		static_cast<InternalHeap*>(heap->GetPrevHeap())->SetNextHeap(heap->GetNextHeap());
	}
	if (heap->GetNextHeap() != nullptr)
	{
		static_cast<InternalHeap*>(heap->GetNextHeap())->SetPrevHeap(heap->GetPrevHeap());
	}
	if (pHeapHead == heap)
	{
		pHeapHead = heap->GetNextHeap();
	}

	Heap::Info heapInfo;
	heap->GetInfo(heapInfo);

	mInfo.CurrBytesUsed -= heapInfo.CurrBytesUsed;
	mInfo.CurrNumAlloc -= heapInfo.CurrNumAlloc;

	heap->Destroy();
	--mInfo.CurrHeapCount;
}

Memory::Code Memory::HeapDestroy(Heap *inHeap)
{
	if (inHeap == nullptr)
	{
		return ERROR_Illegal_Heap;
	}

	Memory* mem = Instance();
	if (!mem->mInitialized)
	{
		return ERROR_Mem_Not_Initialized;
	}

	mem->RemoveHeap(inHeap);
	
	return OK;
}


Memory::Code Memory::GetHeapByAddr(Heap *&pHeap, void *p)
{
	Memory* mem = Instance();
	if (!mem->mInitialized)
	{
		return ERROR_Mem_Not_Initialized;
	}

	InternalHeap* currentHeap = static_cast<InternalHeap*>(mem->pHeapHead);
	while (currentHeap != nullptr)
	{
		if (currentHeap->AddressInHeap(p))
		{
			pHeap = currentHeap;
			return OK;
		}

		currentHeap = static_cast<InternalHeap*>(currentHeap->GetNextHeap());
	}

	return ERROR_Invalid_Addr;
}

// ---  End of File ---------------
