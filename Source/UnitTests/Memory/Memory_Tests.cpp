//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#include "Framework/UnitTest.h"
#include "Memory/MemoryEngine.h"

//---------------------------------------------------------------------------
// Test Files:
// Single step throught, set break points, but do not modify it
//---------------------------------------------------------------------------
TEST(version, mem_group)
{
	CHECK(Memory::MAJOR_VERSION == 2);
	CHECK(Memory::MINOR_VERSION == 0);
}

TEST(No_InitializeMemorySystem, mem_group)
{
	// Initialize the memory system
	//Memory::Initialize();

	// Test the mem Info structures
	Memory::Info mem_Info;

	// get info
	Memory::Code memResult;
	memResult = Memory::GetInfo(mem_Info);

	CHECK(memResult == Memory::ERROR_Mem_Not_Initialized);
}

TEST(No_InitializeDestroy, mem_group)
{
	// Initialize the memory system
	//Memory::Initialize();

	// get info
	Memory::Code memResult;
	memResult = Memory::Destroy();

	CHECK(memResult == Memory::ERROR_Mem_Not_Initialized);
}

TEST(No_InitializeHeapCreate, mem_group)
{
	// Initialize the memory system
	//Memory::Initialize();

	// Create a heap pointer (someday make it a handle)
	Heap *heap_A;

	// create the heap by using the memory system
	Memory::Code memResult;
	memResult = Memory::HeapCreate(heap_A, 2 * 1024 * 1024, "HEAP_A");

	CHECK(memResult == Memory::ERROR_Mem_Not_Initialized);
}

TEST(No_InitializeHeapDestroy, mem_group)
{
	// Initialize the memory system
	//Memory::Initialize();

	// Create a heap pointer (someday make it a handle)
	Heap *heap_A = (Heap *)0x1; //  just for this test

	// create the heap by using the memory system
	Memory::Code memResult;
	memResult = Memory::HeapDestroy(heap_A);

	CHECK(memResult == Memory::ERROR_Mem_Not_Initialized);
}

TEST(No_InitializeGetHeapByAddr, mem_group)
{
	// Initialize the memory system
	//Memory::Initialize();

	// Create a heap pointer (someday make it a handle)
	Heap *heap_A = (Heap *)0x1; //  just for this test

	// create the heap by using the memory system
	Memory::Code memResult;
	memResult = Memory::GetHeapByAddr(heap_A, (void *)0x1);

	CHECK(memResult == Memory::ERROR_Mem_Not_Initialized);
}

TEST(InitializeMemorySystem, mem_group)
{
	// Initialize the memory system
	Memory::Initialize();

	// Test the mem Info structures
	Memory::Info mem_Info;

	// Yes it works.
	Memory::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 0);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 0);
	CHECK(mem_Info.PeakNumAlloc == 0);

	Memory::Destroy();
}


TEST(addOneHeap, mem_group)
{
	// Initialize the memory system
	Memory::Initialize();

	// Test the mem Info structures
	Memory::Info mem_Info;

	// Yes it works.
	Memory::GetInfo(mem_Info);

	// validate it
	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 0);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 0);
	CHECK(mem_Info.PeakNumAlloc == 0);

	// Create a heap pointer (someday make it a handle)
	Heap *heap_A;

	// create the heap by using the memory system
	Memory::Code  memResult;
	memResult = Memory::HeapCreate(heap_A, 2 * 1024 * 1024, "HEAP_A");
	CHECK(memResult == Memory::OK);

	// make sure it Initialized to something
	CHECK(heap_A != 0);

	// Test the heap Info structures
	Heap::Info heapA_Info;

	// Yes it works.
	heap_A->GetInfo(heapA_Info);

	CHECK(heapA_Info.CurrBytesUsed == 0);
	CHECK(heapA_Info.CurrNumAlloc == 0);
	CHECK(heapA_Info.EndAddr == 2 * 1024 * 1024 + (unsigned int)heap_A);
	CHECK(strcmp(heapA_Info.Name, "HEAP_A") == 0);
	CHECK(heapA_Info.StartAddr == (unsigned int)heap_A);
	CHECK(heapA_Info.PeakBytesUsed == 0);
	CHECK(heapA_Info.PeakNumAlloc == 0);
	CHECK(heapA_Info.TotalSize == 2 * 1024 * 1024);

	// Yes it works.
	Memory::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 1);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 1);
	CHECK(mem_Info.PeakNumAlloc == 0);

	Memory::Destroy();
}

TEST(Add4Heaps, mem_group)
{
	// Initialize the memory system
	Memory::Initialize();

	// Test the mem Info structures
	Memory::Info mem_Info;

	// Yes it works.
	Memory::GetInfo(mem_Info);

	// validate it
	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 0);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 0);
	CHECK(mem_Info.PeakNumAlloc == 0);

	// create the heap by using the memory system
	Memory::Code  memResult;
	Heap *heap_A;
	memResult = Memory::HeapCreate(heap_A, 1 * 1024 * 1024, "HEAP_A");
	CHECK(memResult == Memory::OK);

	// make sure it Initialized to something
	CHECK(heap_A != 0);

	// create the heap by using the memory system
	Heap *heap_B;
	memResult = Memory::HeapCreate(heap_B, 2 * 1024 * 1024, "HEAP_B");
	CHECK(memResult == Memory::OK);

	// make sure it Initialized to something
	CHECK(heap_B != 0);

	// create the heap by using the memory system
	Heap *heap_C;
	memResult = Memory::HeapCreate(heap_C, 3 * 1024 * 1024, "HEAP_C");
	CHECK(memResult == Memory::OK);

	// make sure it Initialized to something
	CHECK(heap_C != 0);

	// create the heap by using the memory system
	Heap *heap_D;
	memResult = Memory::HeapCreate(heap_D, 4 * 1024 * 1024, "HEAP_D");
	CHECK(memResult == Memory::OK);

	// make sure it Initialized to something
	CHECK(heap_D != 0);

	// Get the stats
	Memory::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 4);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 4);
	CHECK(mem_Info.PeakNumAlloc == 0);

	// Now walk through the heaps, make sure there is 4 of them

#ifdef _DEBUG
	Heap *hTmp;

	// get the head
	hTmp = Memory::DebugGetHeapHead();

	CHECK(hTmp != 0);

	CHECK(hTmp == heap_D);
	CHECK(hTmp->DebugGetNext() == heap_C);
	CHECK(hTmp->DebugGetPrev() == 0);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_C);
	CHECK(hTmp->DebugGetNext() == heap_B);
	CHECK(hTmp->DebugGetPrev() == heap_D);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_B);
	CHECK(hTmp->DebugGetNext() == heap_A);
	CHECK(hTmp->DebugGetPrev() == heap_C);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_A);
	CHECK(hTmp->DebugGetNext() == 0);
	CHECK(hTmp->DebugGetPrev() == heap_B);
#endif // _DEBUG

	Memory::Destroy();
}

TEST(Add4Heaps_RemoveLastHeap, mem_group)
{
	// Initialize the memory system
	Memory::Initialize();

	// Test the mem Info structures
	Memory::Info mem_Info;

	// Yes it works.
	Memory::GetInfo(mem_Info);

	// create the heap by using the memory system
	Memory::Code memResult;
	Heap *heap_A;
	memResult = Memory::HeapCreate(heap_A, 1 * 1024 * 1024, "HEAP_A");
	CHECK(memResult == Memory::OK);

	// make sure it Initialized to something
	CHECK(heap_A != 0);

	// create the heap by using the memory system
	Heap *heap_B;
	memResult = Memory::HeapCreate(heap_B, 2 * 1024 * 1024, "HEAP_B");
	CHECK(memResult == Memory::OK);

	// make sure it Initialized to something
	CHECK(heap_B != 0);

	// create the heap by using the memory system
	Heap *heap_C;
	memResult = Memory::HeapCreate(heap_C, 3 * 1024 * 1024, "HEAP_C");
	CHECK(memResult == Memory::OK);

	// make sure it Initialized to something
	CHECK(heap_C != 0);

	// create the heap by using the memory system
	Heap *heap_D;
	memResult = Memory::HeapCreate(heap_D, 4 * 1024 * 1024, "HEAP_D");
	CHECK(memResult == Memory::OK);

	// make sure it Initialized to something
	CHECK(heap_D != 0);

	// Get the stats
	Memory::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 4);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 4);
	CHECK(mem_Info.PeakNumAlloc == 0);

#ifdef _DEBUG
	// Now walk through the heaps, make sure there is 4 of them
	Heap *hTmp;

	// get the head
	hTmp = Memory::DebugGetHeapHead();

	CHECK(hTmp != 0);

	CHECK(hTmp == heap_D);
	CHECK(hTmp->DebugGetNext() == heap_C);
	CHECK(hTmp->DebugGetPrev() == 0);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_C);
	CHECK(hTmp->DebugGetNext() == heap_B);
	CHECK(hTmp->DebugGetPrev() == heap_D);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_B);
	CHECK(hTmp->DebugGetNext() == heap_A);
	CHECK(hTmp->DebugGetPrev() == heap_C);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_A);
	CHECK(hTmp->DebugGetNext() == 0);
	CHECK(hTmp->DebugGetPrev() == heap_B);
#endif // _DEBUG

	// Now remove last
	Memory::HeapDestroy(heap_A);

	// Get the stats
	Memory::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 3);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 4);
	CHECK(mem_Info.PeakNumAlloc == 0);

	// Now walk through the heaps, make sure there is 3 of them

#ifdef _DEBUG
// get the head
	hTmp = Memory::DebugGetHeapHead();

	CHECK(hTmp != 0);

	CHECK(hTmp == heap_D);
	CHECK(hTmp->DebugGetNext() == heap_C);
	CHECK(hTmp->DebugGetPrev() == 0);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_C);
	CHECK(hTmp->DebugGetNext() == heap_B);
	CHECK(hTmp->DebugGetPrev() == heap_D);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_B);
	CHECK(hTmp->DebugGetNext() == 0);
	CHECK(hTmp->DebugGetPrev() == heap_C);
#endif // _DEBUG

	Memory::Destroy();
}


TEST(Add4Heaps_RemoveMiddleHeap, mem_group)
{
	// Initialize the memory system
	Memory::Initialize();

	// Test the mem Info structures
	Memory::Info mem_Info;

	// Yes it works.
	Memory::GetInfo(mem_Info);

	Memory::Code memResult;
	// create the heap by using the memory system
	Heap *heap_A;
	memResult = Memory::HeapCreate(heap_A, 1 * 1024 * 1024, "HEAP_A");
	CHECK(memResult == Memory::OK);
	// make sure it Initialized to something
	CHECK(heap_A != 0);

	// create the heap by using the memory system
	Heap *heap_B;
	memResult = Memory::HeapCreate(heap_B, 2 * 1024 * 1024, "HEAP_B");
	CHECK(memResult == Memory::OK);
	// make sure it Initialized to something
	CHECK(heap_B != 0);

	// create the heap by using the memory system
	Heap *heap_C;
	memResult = Memory::HeapCreate(heap_C, 3 * 1024 * 1024, "HEAP_C");
	CHECK(memResult == Memory::OK);
	// make sure it Initialized to something
	CHECK(heap_C != 0);

	// create the heap by using the memory system
	Heap *heap_D;
	memResult = Memory::HeapCreate(heap_D, 4 * 1024 * 1024, "HEAP_D");
	CHECK(memResult == Memory::OK);

	// make sure it Initialized to something
	CHECK(heap_D != 0);

	// Get the stats
	Memory::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 4);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 4);
	CHECK(mem_Info.PeakNumAlloc == 0);

#ifdef _DEBUG
// Now walk through the heaps, make sure there is 4 of them
	Heap *hTmp;

	// get the head
	hTmp = Memory::DebugGetHeapHead();

	CHECK(hTmp != 0);

	CHECK(hTmp == heap_D);
	CHECK(hTmp->DebugGetNext() == heap_C);
	CHECK(hTmp->DebugGetPrev() == 0);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_C);
	CHECK(hTmp->DebugGetNext() == heap_B);
	CHECK(hTmp->DebugGetPrev() == heap_D);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_B);
	CHECK(hTmp->DebugGetNext() == heap_A);
	CHECK(hTmp->DebugGetPrev() == heap_C);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_A);
	CHECK(hTmp->DebugGetNext() == 0);
	CHECK(hTmp->DebugGetPrev() == heap_B);
#endif // _DEBUG

	// Now remove last
	Memory::HeapDestroy(heap_B);

	// Get the stats
	Memory::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 3);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 4);
	CHECK(mem_Info.PeakNumAlloc == 0);

#ifdef _DEBUG
	// Now walk through the heaps, make sure there is 3 of them
	// get the head
	hTmp = Memory::DebugGetHeapHead();

	CHECK(hTmp != 0);

	CHECK(hTmp == heap_D);
	CHECK(hTmp->DebugGetNext() == heap_C);
	CHECK(hTmp->DebugGetPrev() == 0);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_C);
	CHECK(hTmp->DebugGetNext() == heap_A);
	CHECK(hTmp->DebugGetPrev() == heap_D);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_A);
	CHECK(hTmp->DebugGetNext() == 0);
	CHECK(hTmp->DebugGetPrev() == heap_C);
#endif // _DEBUG

	Memory::Destroy();
}


TEST(Add4Heaps_RemoveFirstHeap, mem_group)
{
	// Initialize the memory system
	Memory::Initialize();

	// Test the mem Info structures
	Memory::Info mem_Info;

	// Yes it works.
	Memory::GetInfo(mem_Info);

	Memory::Code memResult;
	// create the heap by using the memory system
	Heap *heap_A;
	memResult = Memory::HeapCreate(heap_A, 1 * 1024 * 1024, "HEAP_A");
	CHECK(memResult == Memory::OK);
	// make sure it Initialized to something
	CHECK(heap_A != 0);

	// create the heap by using the memory system
	Heap *heap_B;
	memResult = Memory::HeapCreate(heap_B, 2 * 1024 * 1024, "HEAP_B");
	CHECK(memResult == Memory::OK);
	// make sure it Initialized to something
	CHECK(heap_B != 0);

	// create the heap by using the memory system
	Heap *heap_C;
	memResult = Memory::HeapCreate(heap_C, 3 * 1024 * 1024, "HEAP_C");
	CHECK(memResult == Memory::OK);
	// make sure it Initialized to something
	CHECK(heap_C != 0);

	// create the heap by using the memory system
	Heap *heap_D;
	memResult = Memory::HeapCreate(heap_D, 4 * 1024 * 1024, "HEAP_D");
	CHECK(memResult == Memory::OK);

	// make sure it Initialized to something
	CHECK(heap_D != 0);

	// Get the stats
	Memory::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 4);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 4);
	CHECK(mem_Info.PeakNumAlloc == 0);

#ifdef _DEBUG
	// Now walk through the heaps, make sure there is 4 of them
	Heap *hTmp;

	// get the head
	hTmp = Memory::DebugGetHeapHead();

	CHECK(hTmp != 0);

	CHECK(hTmp == heap_D);
	CHECK(hTmp->DebugGetNext() == heap_C);
	CHECK(hTmp->DebugGetPrev() == 0);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_C);
	CHECK(hTmp->DebugGetNext() == heap_B);
	CHECK(hTmp->DebugGetPrev() == heap_D);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_B);
	CHECK(hTmp->DebugGetNext() == heap_A);
	CHECK(hTmp->DebugGetPrev() == heap_C);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_A);
	CHECK(hTmp->DebugGetNext() == 0);
	CHECK(hTmp->DebugGetPrev() == heap_B);
#endif // _DEBUG

	// Now remove last
	Memory::HeapDestroy(heap_D);

	// Get the stats
	Memory::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 3);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 4);
	CHECK(mem_Info.PeakNumAlloc == 0);

#ifdef _DEBUG
	// Now walk through the heaps, make sure there is 3 of them
	// get the head
	hTmp = Memory::DebugGetHeapHead();

	CHECK(hTmp != 0);

	CHECK(hTmp == heap_C);
	CHECK(hTmp->DebugGetNext() == heap_B);
	CHECK(hTmp->DebugGetPrev() == 0);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_B);
	CHECK(hTmp->DebugGetNext() == heap_A);
	CHECK(hTmp->DebugGetPrev() == heap_C);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_A);
	CHECK(hTmp->DebugGetNext() == 0);
	CHECK(hTmp->DebugGetPrev() == heap_B);
#endif // _DEBUG

	Memory::Destroy();
}



TEST(Add4Heaps_RemoveAllHeaps, mem_group)
{
	// Initialize the memory system
	Memory::Initialize();

	// Test the mem Info structures
	Memory::Info mem_Info;

	// Yes it works.
	Memory::GetInfo(mem_Info);

	// create the heap by using the memory system
	Memory::Code memResult;
	Heap *heap_A;
	memResult = Memory::HeapCreate(heap_A, 1 * 1024 * 1024, "HEAP_A");
	CHECK(memResult == Memory::OK);

	// make sure it Initialized to something
	CHECK(heap_A != 0);

	// create the heap by using the memory system
	Heap *heap_B;
	memResult = Memory::HeapCreate(heap_B, 2 * 1024 * 1024, "HEAP_B");
	CHECK(memResult == Memory::OK);

	// make sure it Initialized to something
	CHECK(heap_B != 0);

	// create the heap by using the memory system
	Heap *heap_C;
	memResult = Memory::HeapCreate(heap_C, 3 * 1024 * 1024, "HEAP_C");
	CHECK(memResult == Memory::OK);
	// make sure it Initialized to something
	CHECK(heap_C != 0);

	// create the heap by using the memory system
	Heap *heap_D;
	memResult = Memory::HeapCreate(heap_D, 4 * 1024 * 1024, "HEAP_D");
	CHECK(memResult == Memory::OK);
	// make sure it Initialized to something
	CHECK(heap_D != 0);

	// Get the stats
	Memory::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 4);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 4);
	CHECK(mem_Info.PeakNumAlloc == 0);

#ifdef _DEBUG
	// Now walk through the heaps, make sure there is 4 of them
	Heap *hTmp;

	// get the head
	hTmp = Memory::DebugGetHeapHead();

	CHECK(hTmp != 0);

	CHECK(hTmp == heap_D);
	CHECK(hTmp->DebugGetNext() == heap_C);
	CHECK(hTmp->DebugGetPrev() == 0);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_C);
	CHECK(hTmp->DebugGetNext() == heap_B);
	CHECK(hTmp->DebugGetPrev() == heap_D);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_B);
	CHECK(hTmp->DebugGetNext() == heap_A);
	CHECK(hTmp->DebugGetPrev() == heap_C);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_A);
	CHECK(hTmp->DebugGetNext() == 0);
	CHECK(hTmp->DebugGetPrev() == heap_B);
#endif // _DEBUG
	// Now remove last -----------------------------------------------------------
	Memory::HeapDestroy(heap_A);

	// Get the stats
	Memory::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 3);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 4);
	CHECK(mem_Info.PeakNumAlloc == 0);

#ifdef _DEBUG
	// Now walk through the heaps, make sure there is 3 of them
	// get the head
	hTmp = Memory::DebugGetHeapHead();

	CHECK(hTmp != 0);

	CHECK(hTmp == heap_D);
	CHECK(hTmp->DebugGetNext() == heap_C);
	CHECK(hTmp->DebugGetPrev() == 0);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_C);
	CHECK(hTmp->DebugGetNext() == heap_B);
	CHECK(hTmp->DebugGetPrev() == heap_D);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_B);
	CHECK(hTmp->DebugGetNext() == 0);
	CHECK(hTmp->DebugGetPrev() == heap_C);
#endif // _DEBUG

	// Now remove last  ------------------------------------------------
	Memory::HeapDestroy(heap_B);

	// Get the stats
	Memory::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 2);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 4);
	CHECK(mem_Info.PeakNumAlloc == 0);

#ifdef _DEBUG
	// Now walk through the heaps, make sure there is 2 of them
	// get the head
	hTmp = Memory::DebugGetHeapHead();

	CHECK(hTmp != 0);

	CHECK(hTmp == heap_D);
	CHECK(hTmp->DebugGetNext() == heap_C);
	CHECK(hTmp->DebugGetPrev() == 0);

	hTmp = hTmp->DebugGetNext();

	CHECK(hTmp == heap_C);
	CHECK(hTmp->DebugGetNext() == 0);
	CHECK(hTmp->DebugGetPrev() == heap_D);
#endif // _DEBUG

	// Now remove last------------------------------------------------
	Memory::HeapDestroy(heap_C);

	// Get the stats
	Memory::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 1);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 4);
	CHECK(mem_Info.PeakNumAlloc == 0);

#ifdef _DEBUG
	// Now walk through the heaps, make sure there is 1 of them
	// get the head
	hTmp = Memory::DebugGetHeapHead();

	CHECK(hTmp != 0);

	CHECK(hTmp == heap_D);
	CHECK(hTmp->DebugGetNext() == 0);
	CHECK(hTmp->DebugGetPrev() == 0);
#endif // _DEBUG

	// Now remove last------------------------------------------------
	Memory::HeapDestroy(heap_D);

	// Get the stats
	Memory::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 0);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 0);
	CHECK(mem_Info.PeakHeapCount == 4);
	CHECK(mem_Info.PeakNumAlloc == 0);

#ifdef _DEBUG
	// Now walk through the heaps, make sure there is 1 of them
	// get the head
	hTmp = Memory::DebugGetHeapHead();

	CHECK(hTmp == 0);
#endif // _DEBUG

	Memory::Destroy();
}

// ---  End of File ---------------
