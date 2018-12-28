#pragma once

#include "EngineCore/Platform.h"

// forward declaration
class Heap;
class Block;

// Singleton
class Memory
{
public:

	// Constants for the library
	static const uint32 MAJOR_VERSION = 2;
	static const uint32 MINOR_VERSION = 0;

	// alignment 
	enum Align
	{
		Byte_4,
		Byte_8,
		Byte_16,
		Byte_32,
		Byte_64,
		Byte_128,
		Byte_256
	};

	// Return codes
	enum Code
	{
		OK,
		ERROR_Heap_Create,
		ERROR_Illegal_Heap,
		ERROR_Mem_Not_Initialized,
		ERROR_Invalid_Addr,
		Dword = 0x7fffffff
	};

	// info structure
	struct Info
	{
		int32 CurrAllocIndex;
		int32 PeakHeapCount;
		int32 CurrHeapCount;
		int32 PeakNumAlloc;
		int32 CurrNumAlloc;
		int32 PeakBytesUsed;
		int32 CurrBytesUsed;
	};

public:
	// Public Interface ------------------------------   for the customers

	// Initialize the mem system
	static Code Initialize();
	static Code Destroy();

	// Create an Heap from the heap system
	static Code HeapCreate(Heap *&newHeap, uint32 heapSize, const char * const Name);
	static Code HeapCreate(Heap *&newHeap, uint32 numBlocks, uint32 sizePerBlock, const char * const Name);

	static void* Malloc(size_t inSize, Heap *pHeap, Memory::Align align, char *inName, int lineNum);
	static void Free(void* data, Heap* heap);

	// Create an Heap from the heap system
	static Code HeapDestroy(Heap *inHeap);

	// Information
	static Code GetInfo(Info &info);
	static Code GetHeapByAddr(Heap *&pHeap, void *p);

#ifdef _DEBUG
	static Block *DebugGetGlobalTrackingHead(void);
	// Needed for Testings (Not for Customers)
	static Heap *DebugGetHeapHead(void);
#endif

	// -----------------------------------------------------------------------------------
	// Add extra data or methods below this line
	// -----------------------------------------------------------------------------------

protected:

#ifdef _DEBUG
	void AddAllocation(Block* block);
	void RemoveAllocation(Block* block);
	void CreateDebugHeap();
#endif

	void RemoveHeapAllocation(Block* block);
	void RemoveHeap(Heap* heap);

	Memory() = default;
	~Memory() = default;

	static Memory* Instance();

	// data -----------------------------------------------------------------------
	Heap	*debugBlocks;
	Heap	*pHeapHead;
	Block	*pGlobalHead;
	Info	mInfo;
	bool	mInitialized;
	char	pad[3];
};
