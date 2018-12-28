#pragma once

#include "EngineCore/Platform.h"
#include "InternalHeap.h"

// TODO - Write unit tests for fixed block
class FixedBlockHeap final : public Internal::InternalHeap
{
public:
	FixedBlockHeap(Heap* nextHeap, const char * const heapName, uint32 numBlocks, uint32 sizePerBlock, HANDLE heapHandle, Memory* memEngine);
	~FixedBlockHeap();
	FixedBlockHeap(const FixedBlockHeap&) = delete;
	FixedBlockHeap& operator=(const FixedBlockHeap& otherHeap) = delete;

protected:
	virtual void* MallocInternal(size_t inSize, uint32 align, char *inName, int32 lineNum, int32 allocationIndex) override;
	virtual void FreeInternal(void* memory) override;
	virtual void DestroyInternal() override;

#ifdef _DEBUG
	// Tracking block 
	virtual Block *GetTrackingHead() override { return nullptr; }
#endif

private:
	struct FixedBlock
	{
		FixedBlock* next = nullptr;
	};

	FixedBlock* blockHead;
	const uint32 maxNumBlocks;
	const uint32 sizeOfBlock;
	uint32 currentNumBlocks;
};