#pragma once

#include "InternalHeap.h"

class Block;

class DynamicBlockHeap final : public Internal::InternalHeap
{
public:
	DynamicBlockHeap(const char* heapName, size_t heapSize, Heap* nextHeap, HANDLE heapHandle, Memory* memEngine);
	~DynamicBlockHeap();
	DynamicBlockHeap(const DynamicBlockHeap&) = delete;
	DynamicBlockHeap& operator=(const DynamicBlockHeap&) = delete;

#ifdef _DEBUG
	virtual Block* GetTrackingHead() override;
#endif // _DEBUG
	
protected:
	virtual void* MallocInternal(size_t inSize, uint32 align, char *inName, int32 lineNum, int32 allocationIndex) override;
	virtual void FreeInternal(void* memory) override;
	virtual void DestroyInternal() override;

private:
	Block	*pBlkHead;
};