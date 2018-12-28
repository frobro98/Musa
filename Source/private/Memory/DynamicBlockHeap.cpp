
#include "EngineCore/Platform.h"
#include "DynamicBlockHeap.h"
#include "MemoryUtilities.h"
#include "InternalBlock.h"
#include "InternalMem.h"

using namespace Internal;

DynamicBlockHeap::DynamicBlockHeap(const char * heapName, size_t heapSize, Heap * nextHeap, HANDLE heapHandle, Memory * memEngine)
	:Internal::InternalHeap(heapName, heapSize, nextHeap, heapHandle, memEngine)
{
	mInfo.mType = Heap::Type::Normal;
}

DynamicBlockHeap::~DynamicBlockHeap()
{
}

void * DynamicBlockHeap::MallocInternal(size_t inSize, uint32 align, char *inName, int32 lineNum, int32 allocationIndex)
{
	assert(IsPowerOf2(align));

	// Update Info
	mInfo.CurrBytesUsed += inSize;
	++mInfo.CurrNumAlloc;
	if (mInfo.CurrBytesUsed > mInfo.PeakBytesUsed)
	{
		mInfo.PeakBytesUsed = mInfo.CurrBytesUsed;
	}
	if (mInfo.CurrNumAlloc > mInfo.PeakNumAlloc)
	{
		mInfo.PeakNumAlloc = mInfo.CurrNumAlloc;
	}

	size_t actualMemSize = inSize + sizeof(Block) + align;

	void* data = HeapAlloc(mWinHeapHandle, HEAP_ZERO_MEMORY, actualMemSize);
	if ((reinterpret_cast<uint32>(data)) < mInfo.EndAddr)
	{
		InternalBlock* trackingBlock = new(data) InternalBlock(inName, lineNum, inSize, allocationIndex);

		// Update the block list
		if (pBlkHead)
		{
			trackingBlock->SetHeapNext(pBlkHead);
			(static_cast<InternalBlock*>(pBlkHead))->SetHeapPrev(trackingBlock);
		}
		pBlkHead = trackingBlock;
		void* actualData = reinterpret_cast<void*>(reinterpret_cast<unsigned int>(trackingBlock) + sizeof(Block));
		actualData = Align(actualData, align);

		uint32* secretPointer = reinterpret_cast<uint32*>(reinterpret_cast<uint32>(actualData) - sizeof(size_t));
		*secretPointer = reinterpret_cast<uint32>(trackingBlock);
		return actualData;
	}
	else
	{
		HeapFree(mWinHeapHandle, 0, data);
		return nullptr;
	}
}

void DynamicBlockHeap::FreeInternal(void * memory)
{
	InternalBlock* block = reinterpret_cast<InternalBlock*>(GetSecretPointer(memory));
	unsigned int blockSize = block->GetAllocSize();

	InternalBlock* pHeapNext = static_cast<InternalBlock*>(block->GetHeapNext());
	InternalBlock* pHeapPrev = static_cast<InternalBlock*>(block->GetHeapPrev());

	if (pHeapNext != nullptr)
	{
		pHeapNext->SetHeapPrev(pHeapPrev);
	}
	if (pHeapPrev != nullptr)
	{
		pHeapPrev->SetHeapNext(pHeapNext);
	}

	mInfo.CurrBytesUsed -= blockSize;
	--mInfo.CurrNumAlloc;

	if (pBlkHead == block)
	{
		pBlkHead = block->GetHeapNext();
	}

	HeapFree(mWinHeapHandle, 0, (void*)block);
}

void DynamicBlockHeap::DestroyInternal()
{
	// Find memory leaks
	Block* current = pBlkHead;
	while (current != nullptr)
	{
		// Disconnect from global
		if (current->GetGlobalNext() != nullptr)
		{
			(static_cast<InternalBlock*>(current->GetGlobalNext()))->SetGlobalPrev(current->GetGlobalPrev());
		}
		if (current->GetGlobalPrev() != nullptr)
		{
			(static_cast<InternalBlock*>(current->GetGlobalPrev()))->SetGlobalNext(current->GetGlobalNext());
		}
		if ((static_cast<InternalMem*>(pMem))->GetGlobalHead() == current)
		{
			(static_cast<InternalMem*>(pMem))->SetGlobalHead(current->GetGlobalNext());
		}

		// TODO - Add memory tracking
		// print memory leak information
		current = current->GetHeapNext();
		// Inform user of mem leak!
	}

	HeapDestroy(mWinHeapHandle);
}

#ifdef _DEBUG
Block *DynamicBlockHeap::GetTrackingHead()
{
	return pBlkHead;
}
#endif // _DEBUG
