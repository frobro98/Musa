#include "FixedBlockHeap.h"
#include "Assertion.h"

FixedBlockHeap::FixedBlockHeap(Heap* nextHeap, const char * const heapName, uint32 numBlocks, uint32 sizePerBlock, HANDLE heapHandle, Memory* memEngine)
	: Internal::InternalHeap(heapName, (size_t)(numBlocks*sizePerBlock), nextHeap, heapHandle, memEngine),
	maxNumBlocks(numBlocks),
	sizeOfBlock(sizePerBlock),
	currentNumBlocks(0)
{
	// Setting up initial information
	uint8* heapAddr = reinterpret_cast<uint8*>(this);
	uint8* blockAddr = heapAddr + sizeof(FixedBlock);
	blockHead = reinterpret_cast<FixedBlock*>(blockAddr);

	// Walking memory set up
	uint32 numBlocksLeft = numBlocks-1;
	FixedBlock* prevBlock = blockHead;
	blockAddr = blockAddr + sizePerBlock;

	while (numBlocksLeft > 0)
	{
		FixedBlock* block = reinterpret_cast<FixedBlock*>(blockAddr);
		prevBlock->next = block;
		prevBlock = block;

		blockAddr = blockAddr + sizePerBlock;
		--numBlocksLeft;
	}

	prevBlock->next = nullptr;
	mInfo.type = Heap::Type::Fixed;
}

FixedBlockHeap::~FixedBlockHeap()
{
}

void * FixedBlockHeap::MallocInternal(uint64 inSize, uint32 /*align*/, const char * /*inName*/, uint32 /*lineNum*/, uint32 /*allocationIndex*/)
{
#ifdef _UNIT_TEST
	if (inSize != sizeOfBlock)
	{
		return nullptr;
	}
#else
	Assert(inSize == sizeOfBlock);
	UNUSED(inSize);
#endif
	// Update heap information
	mInfo.CurrNumAlloc += 1;
	if (mInfo.CurrNumAlloc > mInfo.PeakNumAlloc)
	{
		mInfo.PeakNumAlloc = mInfo.CurrNumAlloc;
	}

	mInfo.CurrBytesUsed += sizeOfBlock;

	if(mInfo.CurrBytesUsed > mInfo.PeakBytesUsed)
	{
		mInfo.PeakBytesUsed = mInfo.CurrBytesUsed;
	}
	--currentNumBlocks;

	if (currentNumBlocks > 0)
	{
		FixedBlock* block = blockHead;
		blockHead = block->next;
		return block;
	}

	// TODO - actually get better logging for assertions
	Assert(false);
	return nullptr;
}

void FixedBlockHeap::FreeInternal(void * memory)
{
	// Update heap information
	--mInfo.CurrNumAlloc;
	mInfo.CurrBytesUsed -= sizeOfBlock;
	++currentNumBlocks;

	FixedBlock* block = reinterpret_cast<FixedBlock*>(memory);
	block->next = blockHead;
	blockHead = block;
}

void FixedBlockHeap::DestroyInternal()
{
}