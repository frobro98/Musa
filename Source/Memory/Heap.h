#pragma once

#include "EngineCore/Platform.h"
#include "EngineCore/MemoryUtilities.h"

// Forward declaration
class Block;
class Memory;


// TODO - Make this abstract and make a StaticBlockHeap and DynamicBlockHeap
// Heap class
class Heap 
{
public:
	// Constants 
	static const uint32 NameLength = 32;
	static const uint32 NameNumChar = (NameLength - 1);

	// Heap Types
	enum class Type : uint32
	{
		Normal,
		Fixed
	};

	// Info structure
	struct Info
	{
		char Name[Heap::NameLength];
		uint32 StartAddr;
		uint32 EndAddr;
		uint32 TotalSize;
		int32 PeakNumAlloc;
		int32 CurrNumAlloc;
		int32 PeakBytesUsed;
		int32 CurrBytesUsed;
		Type	mType;

		Info()
			:StartAddr(0),
			EndAddr(0),
			TotalSize(0),
			PeakNumAlloc(0),
			CurrNumAlloc(0),
			PeakBytesUsed(0),
			CurrBytesUsed(0)
		{
			Memset(Name, 0, Heap::NameLength);
		}
	};

public:
	// Public Interface (For Customers)
	void GetInfo(Info &info);

	// TODO - Remove this outside of debug reasons
#ifdef _DEBUG
	// Tracking block 
	virtual Block *GetTrackingHead() = 0;
	// Needed for Testings (Not for Customers)
	Heap *DebugGetNext() const;
	Heap *DebugGetPrev() const;
#endif // _DEBUG

protected:

	Heap(const char* heapName, size_t heapSize, Heap* nextHeap, HANDLE heapHandle, Memory* memEngine);
	virtual ~Heap();

	virtual void* MallocInternal(size_t inSize, uint32 align, char *inName, int32 lineNum, int32 allocationIndex) = 0;
	virtual void FreeInternal(void* memory) = 0;
	virtual void DestroyInternal() = 0;

	// data -----------------------------------------------------------------------
	Info	mInfo;
	Memory     *pMem;
	HANDLE  mWinHeapHandle;
	Heap* heapNext;
	Heap* heapPrev;

};
