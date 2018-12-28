//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#include "Framework/UnitTest.h"
#include "Memory/MemoryEngine.h"
#include "Dog.h"

//---------------------------------------------------------------------------
// Test Files:
// Single step throught, set break points, but do not modify it
//---------------------------------------------------------------------------

TEST( Add5Allocs_TrackingThroughHeap, Tracking_Global_group)
{
	   // Initialize the memory system
	Memory::Initialize();

	// Test the mem Info structures
	Memory::Info mem_Info;

	// Yes it works.
	Memory::GetInfo( mem_Info );

   // validate it
   CHECK( mem_Info.CurrBytesUsed == 0 );
   CHECK( mem_Info.CurrHeapCount == 0 );
   CHECK( mem_Info.CurrNumAlloc == 0 );
   CHECK( mem_Info.PeakBytesUsed == 0 );
   CHECK( mem_Info.PeakHeapCount == 0 );
   CHECK( mem_Info.PeakNumAlloc == 0 );

	// Create a heap pointer (someday make it a handle)
	Heap *heap_A;

	// create the heap by using the memory system
	Memory::Code memResult;
	memResult = Memory::HeapCreate( heap_A, 2*1024*1024, "HEAP_A" );
	CHECK( memResult == Memory::OK);

   // make sure it Initialized to something
   CHECK( heap_A != 0 );

	// Test the heap Info structures
	Heap::Info heapA_Info;

	// Yes it works.
	heap_A->GetInfo( heapA_Info );

   CHECK( heapA_Info.CurrBytesUsed == 0 );
   CHECK( heapA_Info.CurrNumAlloc == 0 );
   CHECK( heapA_Info.EndAddr == 2*1024*1024 + (unsigned int)heap_A );
   CHECK( strcmp( heapA_Info.Name,"HEAP_A") == 0 );
   CHECK( heapA_Info.StartAddr == (unsigned int)heap_A );
   CHECK( heapA_Info.PeakBytesUsed == 0 );
   CHECK( heapA_Info.PeakNumAlloc == 0 );
   CHECK( heapA_Info.TotalSize == 2*1024*1024 );

	// Yes it works.
	Memory::GetInfo( mem_Info );

   CHECK( mem_Info.CurrBytesUsed == 0 );
   CHECK( mem_Info.CurrHeapCount == 1 );
   CHECK( mem_Info.CurrNumAlloc == 0 );
   CHECK( mem_Info.PeakBytesUsed == 0 );
   CHECK( mem_Info.PeakHeapCount == 1 );
   CHECK( mem_Info.PeakNumAlloc == 0 );

   // OK at this point, we have a heap create, so let's allocate from it.

   Dog *p0 = new(heap_A,Memory::Align::Byte_4,__FILE__,__LINE__) Dog();
   Dog *p1 = new(heap_A,Memory::Align::Byte_4,__FILE__,__LINE__) Dog();
   Dog *p2 = new(heap_A,Memory::Align::Byte_4,__FILE__,__LINE__) Dog();
   Dog *p3 = new(heap_A,Memory::Align::Byte_4,__FILE__,__LINE__) Dog();
   Dog *p4 = new(heap_A,Memory::Align::Byte_4,__FILE__,__LINE__) Dog();

   	// Yes it works.
	Memory::GetInfo( mem_Info );

   CHECK( mem_Info.CurrBytesUsed == 5*sizeof(Dog) );
   CHECK( mem_Info.CurrHeapCount == 1 );
   CHECK( mem_Info.CurrNumAlloc == 5 );
   CHECK( mem_Info.PeakBytesUsed == 5*sizeof(Dog) );
   CHECK( mem_Info.PeakHeapCount == 1 );
   CHECK( mem_Info.PeakNumAlloc == 5 );

#ifdef _DEBUG
   // OK, now all 5 allocations are created, now look at track info from heap.
   Block *t0 = Memory::DebugGetGlobalTrackingHead();
   
   CHECK( t0 != 0 );
   CHECK( t0->GetAllocIndex() == 5 );
   CHECK( strcmp(t0->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t0->GetLineNum() == 76);
   CHECK( t0->GetGlobalPrev() == 0 );
   CHECK( t0->GetGlobalNext() != 0 );

   Block *t1 = t0->GetGlobalNext();

   CHECK( t1 != 0 );
   CHECK( t1->GetAllocIndex() == 4 );
   CHECK( strcmp(t1->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t1->GetLineNum() == 75);
   CHECK( t1->GetGlobalPrev() == t0 );
   CHECK( t1->GetGlobalNext() != 0 );
   CHECK( t0->GetGlobalNext() == t1 );

   Block *t2 = t1->GetGlobalNext();

   CHECK( t2 != 0 );
   CHECK( t2->GetAllocIndex() == 3 );
   CHECK( strcmp(t2->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t2->GetLineNum() == 74);
   CHECK( t2->GetGlobalPrev() == t1 );
   CHECK( t2->GetGlobalNext() != 0 );
   CHECK( t1->GetGlobalNext() == t2 );

   Block *t3 = t2->GetGlobalNext();

   CHECK( t3 != 0 );
   CHECK( t3->GetAllocIndex() == 2 );
   CHECK( strcmp(t3->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t3->GetLineNum() == 73);
   CHECK( t3->GetGlobalPrev() == t2 );
   CHECK( t3->GetGlobalNext() != 0 );
   CHECK( t2->GetGlobalNext() == t3 );

   Block *t4 = t3->GetGlobalNext();

   CHECK( t4 != 0 );
   CHECK( t4->GetAllocIndex() == 1 );
   CHECK( strcmp(t4->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t4->GetLineNum() == 72);
   CHECK( t4->GetGlobalPrev() == t3 );
   CHECK( t4->GetGlobalNext() == 0 );
   CHECK( t3->GetGlobalNext() == t4 );
#endif // _DEBUG

   delete p0;
   delete p1;
   delete p2;
   delete p3;
   delete p4;
}



TEST( Add4Allocs_RemoveLast_TrackingThroughHeap, Tracking_Global_group)
{
	   // Initialize the memory system
	Memory::Initialize();

	// Test the mem Info structures
	Memory::Info mem_Info;

	// Yes it works.
	Memory::GetInfo( mem_Info );

   // validate it
   CHECK( mem_Info.CurrBytesUsed == 0 );
   CHECK( mem_Info.CurrHeapCount == 0 );
   CHECK( mem_Info.CurrNumAlloc == 0 );
   CHECK( mem_Info.PeakBytesUsed == 0 );
   CHECK( mem_Info.PeakHeapCount == 0 );
   CHECK( mem_Info.PeakNumAlloc == 0 );

	// Create a heap pointer (someday make it a handle)
	Heap *heap_A;

	// create the heap by using the memory system
	Memory::Code memResult;
	memResult = Memory::HeapCreate(heap_A, 2*1024*1024, "HEAP_A" );
	CHECK( memResult == Memory::OK );

   // make sure it Initialized to something
   CHECK( heap_A != 0 );

	// Test the heap Info structures
	Heap::Info heapA_Info;

	// Yes it works.
	heap_A->GetInfo( heapA_Info );

   CHECK( heapA_Info.CurrBytesUsed == 0 );
   CHECK( heapA_Info.CurrNumAlloc == 0 );
   CHECK( heapA_Info.EndAddr == 2*1024*1024 + (unsigned int)heap_A );
   CHECK( strcmp( heapA_Info.Name,"HEAP_A") == 0 );
   CHECK( heapA_Info.StartAddr == (unsigned int)heap_A );
   CHECK( heapA_Info.PeakBytesUsed == 0 );
   CHECK( heapA_Info.PeakNumAlloc == 0 );
   CHECK( heapA_Info.TotalSize == 2*1024*1024 );

	// Yes it works.
	Memory::GetInfo( mem_Info );

   CHECK( mem_Info.CurrBytesUsed == 0 );
   CHECK( mem_Info.CurrHeapCount == 1 );
   CHECK( mem_Info.CurrNumAlloc == 0 );
   CHECK( mem_Info.PeakBytesUsed == 0 );
   CHECK( mem_Info.PeakHeapCount == 1 );
   CHECK( mem_Info.PeakNumAlloc == 0 );

   // OK at this point, we have a heap create, so let's allocate from it.

   Dog *p0 = new(heap_A,Memory::Align::Byte_4,__FILE__,__LINE__) Dog();
   Dog *p1 = new(heap_A,Memory::Align::Byte_4,__FILE__,__LINE__) Dog();
   Dog *p2 = new(heap_A,Memory::Align::Byte_4,__FILE__,__LINE__) Dog();
   Dog *p3 = new(heap_A,Memory::Align::Byte_4,__FILE__,__LINE__) Dog();

   	// Yes it works.
	Memory::GetInfo( mem_Info );

   CHECK( mem_Info.CurrBytesUsed == 4*sizeof(Dog) );
   CHECK( mem_Info.CurrHeapCount == 1 );
   CHECK( mem_Info.CurrNumAlloc == 4 );
   CHECK( mem_Info.PeakBytesUsed == 4*sizeof(Dog) );
   CHECK( mem_Info.PeakHeapCount == 1 );
   CHECK( mem_Info.PeakNumAlloc == 4 );

#ifdef _DEBUG
   // OK, now all 5 allocations are created, now look at track info from heap.
   Block *t0 = Memory::DebugGetGlobalTrackingHead();
   
   CHECK( t0 != 0 );
   CHECK( t0->GetAllocIndex() == 4 );
   CHECK( strcmp(t0->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t0->GetLineNum() == 209);
   CHECK( t0->GetGlobalPrev() == 0 );
   CHECK( t0->GetGlobalNext() != 0 );

   Block *t1 = t0->GetGlobalNext();

   CHECK( t1 != 0 );
   CHECK( t1->GetAllocIndex() == 3 );
   CHECK( strcmp(t1->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t1->GetLineNum() == 208);
   CHECK( t1->GetGlobalPrev() == t0 );
   CHECK( t1->GetGlobalNext() != 0 );
   CHECK( t0->GetGlobalNext() == t1 );

   Block *t2 = t1->GetGlobalNext();

   CHECK( t2 != 0 );
   CHECK( t2->GetAllocIndex() == 2 );
   CHECK( strcmp(t2->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t2->GetLineNum() == 207);
   CHECK( t2->GetGlobalPrev() == t1 );
   CHECK( t2->GetGlobalNext() != 0 );
   CHECK( t1->GetGlobalNext() == t2 );

   Block *t3 = t2->GetGlobalNext();

   CHECK( t3 != 0 );
   CHECK( t3->GetAllocIndex() == 1 );
   CHECK( strcmp(t3->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t3->GetLineNum() == 206);
   CHECK( t3->GetGlobalPrev() == t2 );
   CHECK( t3->GetGlobalNext() == 0 );
   CHECK( t2->GetGlobalNext() == t3 );

#endif // _DEBUG
   // Now remove the last one --------------------------------------------
   
   delete p0;

   // Yes it works.
	Memory::GetInfo( mem_Info );

   CHECK( mem_Info.CurrBytesUsed == 3*sizeof(Dog) );
   CHECK( mem_Info.CurrHeapCount == 1 );
   CHECK( mem_Info.CurrNumAlloc == 3 );
   CHECK( mem_Info.PeakBytesUsed == 4*sizeof(Dog) );
   CHECK( mem_Info.PeakHeapCount == 1 );
   CHECK( mem_Info.PeakNumAlloc == 4 );

#ifdef _DEBUG
   // OK, now all 4 allocations are created 1 deleted, now look at track info from heap.
   t0 = Memory::DebugGetGlobalTrackingHead();
   
   CHECK( t0 != 0 );
   CHECK( t0->GetAllocIndex() == 4 );
   CHECK( strcmp(t0->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t0->GetLineNum() == 209);
   CHECK( t0->GetGlobalPrev() == 0 );
   CHECK( t0->GetGlobalNext() != 0 );

   t1 = t0->GetGlobalNext();

   CHECK( t1 != 0 );
   CHECK( t1->GetAllocIndex() == 3 );
   CHECK( strcmp(t1->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t1->GetLineNum() == 208);
   CHECK( t1->GetGlobalPrev() == t0 );
   CHECK( t1->GetGlobalNext() != 0 );
   CHECK( t0->GetGlobalNext() == t1 );

   t2 = t1->GetGlobalNext();

   CHECK( t2 != 0 );
   CHECK( t2->GetAllocIndex() == 2 );
   CHECK( strcmp(t2->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t2->GetLineNum() == 207);
   CHECK( t2->GetGlobalPrev() == t1 );
   CHECK( t2->GetGlobalNext() == 0 );
   CHECK( t1->GetGlobalNext() == t2 );

   t3 = t2->GetGlobalNext();

   CHECK( t3 == 0 );
#endif // _DEBUG

   delete p1;
   delete p2;
   delete p3;
}


TEST( Add4Allocs_RemoveFirst_TrackingThroughHeap, Tracking_Global_group)
{
	   // Initialize the memory system
	Memory::Initialize();

	// Test the mem Info structures
	Memory::Info mem_Info;

	// Yes it works.
	Memory::GetInfo( mem_Info );

   // validate it
   CHECK( mem_Info.CurrBytesUsed == 0 );
   CHECK( mem_Info.CurrHeapCount == 0 );
   CHECK( mem_Info.CurrNumAlloc == 0 );
   CHECK( mem_Info.PeakBytesUsed == 0 );
   CHECK( mem_Info.PeakHeapCount == 0 );
   CHECK( mem_Info.PeakNumAlloc == 0 );

	// Create a heap pointer (someday make it a handle)
	Heap *heap_A;

	// create the heap by using the memory system
	Memory::Code memResult;
	memResult = Memory::HeapCreate(heap_A, 2*1024*1024, "HEAP_A" );
	CHECK( memResult == Memory::OK);

   // make sure it Initialized to something
   CHECK( heap_A != 0 );

	// Test the heap Info structures
	Heap::Info heapA_Info;

	// Yes it works.
	heap_A->GetInfo( heapA_Info );

   CHECK( heapA_Info.CurrBytesUsed == 0 );
   CHECK( heapA_Info.CurrNumAlloc == 0 );
   CHECK( heapA_Info.EndAddr == 2*1024*1024 + (unsigned int)heap_A );
   CHECK( strcmp( heapA_Info.Name,"HEAP_A") == 0 );
   CHECK( heapA_Info.StartAddr == (unsigned int)heap_A );
   CHECK( heapA_Info.PeakBytesUsed == 0 );
   CHECK( heapA_Info.PeakNumAlloc == 0 );
   CHECK( heapA_Info.TotalSize == 2*1024*1024 );

	// Yes it works.
	Memory::GetInfo( mem_Info );

   CHECK( mem_Info.CurrBytesUsed == 0 );
   CHECK( mem_Info.CurrHeapCount == 1 );
   CHECK( mem_Info.CurrNumAlloc == 0 );
   CHECK( mem_Info.PeakBytesUsed == 0 );
   CHECK( mem_Info.PeakHeapCount == 1 );
   CHECK( mem_Info.PeakNumAlloc == 0 );

   // OK at this point, we have a heap create, so let's allocate from it.

   Dog *p0 = new(heap_A,Memory::Align::Byte_4,__FILE__,__LINE__) Dog();
   Dog *p1 = new(heap_A,Memory::Align::Byte_4,__FILE__,__LINE__) Dog();
   Dog *p2 = new(heap_A,Memory::Align::Byte_4,__FILE__,__LINE__) Dog();
   Dog *p3 = new(heap_A,Memory::Align::Byte_4,__FILE__,__LINE__) Dog();

   	// Yes it works.
	Memory::GetInfo( mem_Info );

   CHECK( mem_Info.CurrBytesUsed == 4*sizeof(Dog) );
   CHECK( mem_Info.CurrHeapCount == 1 );
   CHECK( mem_Info.CurrNumAlloc == 4 );
   CHECK( mem_Info.PeakBytesUsed == 4*sizeof(Dog) );
   CHECK( mem_Info.PeakHeapCount == 1 );
   CHECK( mem_Info.PeakNumAlloc == 4 );

#ifdef _DEBUG
   // OK, now all 4 allocations are created, now look at track info from heap.
   Block *t0 = Memory::DebugGetGlobalTrackingHead();
   
   CHECK( t0 != 0 );
   CHECK( t0->GetAllocIndex() == 4 );
   CHECK( strcmp(t0->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t0->GetLineNum() == 379);
   CHECK( t0->GetGlobalPrev() == 0 );
   CHECK( t0->GetGlobalNext() != 0 );

   Block *t1 = t0->GetGlobalNext();

   CHECK( t1 != 0 );
   CHECK( t1->GetAllocIndex() == 3 );
   CHECK( strcmp(t1->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t1->GetLineNum() == 378);
   CHECK( t1->GetGlobalPrev() == t0 );
   CHECK( t1->GetGlobalNext() != 0 );
   CHECK( t0->GetGlobalNext() == t1 );

   Block *t2 = t1->GetGlobalNext();

   CHECK( t2 != 0 );
   CHECK( t2->GetAllocIndex() == 2 );
   CHECK( strcmp(t2->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t2->GetLineNum() == 377);
   CHECK( t2->GetGlobalPrev() == t1 );
   CHECK( t2->GetGlobalNext() != 0 );
   CHECK( t1->GetGlobalNext() == t2 );

   Block *t3 = t2->GetGlobalNext();

   CHECK( t3 != 0 );
   CHECK( t3->GetAllocIndex() == 1 );
   CHECK( strcmp(t3->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t3->GetLineNum() == 376);
   CHECK( t3->GetGlobalPrev() == t2 );
   CHECK( t3->GetGlobalNext() == 0 );
   CHECK( t2->GetGlobalNext() == t3 );
#endif // _DEBUG

   // Now remove the last one --------------------------------------------
   
   delete p3;

   // Yes it works.
	Memory::GetInfo( mem_Info );

   CHECK( mem_Info.CurrBytesUsed == 3*sizeof(Dog) );
   CHECK( mem_Info.CurrHeapCount == 1 );
   CHECK( mem_Info.CurrNumAlloc == 3 );
   CHECK( mem_Info.PeakBytesUsed == 4*sizeof(Dog) );
   CHECK( mem_Info.PeakHeapCount == 1 );
   CHECK( mem_Info.PeakNumAlloc == 4 );

#ifdef _DEBUG
   // OK, now all 4 allocations are created 1 deleted, now look at track info from heap.
   t0 = Memory::DebugGetGlobalTrackingHead();
   
   CHECK( t0 != 0 );
   CHECK( t0->GetAllocIndex() == 3 );
   CHECK( strcmp(t0->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t0->GetLineNum() == 378);
   CHECK( t0->GetGlobalPrev() == 0 );
   CHECK( t0->GetGlobalNext() != 0 );

   t1 = t0->GetGlobalNext();

   CHECK( t1 != 0 );
   CHECK( t1->GetAllocIndex() == 2 );
   CHECK( strcmp(t1->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t1->GetLineNum() == 377);
   CHECK( t1->GetGlobalPrev() == t0 );
   CHECK( t1->GetGlobalNext() != 0 );
   CHECK( t0->GetGlobalNext() == t1 );

   t2 = t1->GetGlobalNext();

   CHECK( t2 != 0 );
   CHECK( t2->GetAllocIndex() == 1 );
   CHECK( strcmp(t2->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t2->GetLineNum() == 376);
   CHECK( t2->GetGlobalPrev() == t1 );
   CHECK( t2->GetGlobalNext() == 0 );
   CHECK( t1->GetGlobalNext() == t2 );

   t3 = t2->GetGlobalNext();

   CHECK( t3 == 0 );
#endif // _DEBUG

   delete p0;
   delete p1;
   delete p2;

}


TEST( Add4Allocs_RemoveMiddle_TrackingThroughHeap, Tracking_Global_group)
{
	   // Initialize the memory system
	Memory::Initialize();

	// Test the mem Info structures
	Memory::Info mem_Info;

	// Yes it works.
	Memory::GetInfo( mem_Info );

   // validate it
   CHECK( mem_Info.CurrBytesUsed == 0 );
   CHECK( mem_Info.CurrHeapCount == 0 );
   CHECK( mem_Info.CurrNumAlloc == 0 );
   CHECK( mem_Info.PeakBytesUsed == 0 );
   CHECK( mem_Info.PeakHeapCount == 0 );
   CHECK( mem_Info.PeakNumAlloc == 0 );

	// Create a heap pointer (someday make it a handle)
	Heap *heap_A;

	// create the heap by using the memory system
	Memory::Code memResult;
	memResult = Memory::HeapCreate(heap_A, 2*1024*1024, "HEAP_A" );
	CHECK( memResult == Memory::OK );

   // make sure it Initialized to something
   CHECK( heap_A != 0 );

	// Test the heap Info structures
	Heap::Info heapA_Info;

	// Yes it works.
	heap_A->GetInfo( heapA_Info );

   CHECK( heapA_Info.CurrBytesUsed == 0 );
   CHECK( heapA_Info.CurrNumAlloc == 0 );
   CHECK( heapA_Info.EndAddr == 2*1024*1024 + (unsigned int)heap_A );
   CHECK( strcmp( heapA_Info.Name,"HEAP_A") == 0 );
   CHECK( heapA_Info.StartAddr == (unsigned int)heap_A );
   CHECK( heapA_Info.PeakBytesUsed == 0 );
   CHECK( heapA_Info.PeakNumAlloc == 0 );
   CHECK( heapA_Info.TotalSize == 2*1024*1024 );

	// Yes it works.
	Memory::GetInfo( mem_Info );

   CHECK( mem_Info.CurrBytesUsed == 0 );
   CHECK( mem_Info.CurrHeapCount == 1 );
   CHECK( mem_Info.CurrNumAlloc == 0 );
   CHECK( mem_Info.PeakBytesUsed == 0 );
   CHECK( mem_Info.PeakHeapCount == 1 );
   CHECK( mem_Info.PeakNumAlloc == 0 );

   // OK at this point, we have a heap create, so let's allocate from it.

   Dog *p0 = new(heap_A,Memory::Align::Byte_4,__FILE__,__LINE__) Dog();
   Dog *p1 = new(heap_A,Memory::Align::Byte_4,__FILE__,__LINE__) Dog();
   Dog *p2 = new(heap_A,Memory::Align::Byte_4,__FILE__,__LINE__) Dog();
   Dog *p3 = new(heap_A,Memory::Align::Byte_4,__FILE__,__LINE__) Dog();

   	// Yes it works.
	Memory::GetInfo( mem_Info );

   CHECK( mem_Info.CurrBytesUsed == 4*sizeof(Dog) );
   CHECK( mem_Info.CurrHeapCount == 1 );
   CHECK( mem_Info.CurrNumAlloc == 4 );
   CHECK( mem_Info.PeakBytesUsed == 4*sizeof(Dog) );
   CHECK( mem_Info.PeakHeapCount == 1 );
   CHECK( mem_Info.PeakNumAlloc == 4 );

#ifdef _DEBUG
   // OK, now all 4 allocations are created, now look at track info from heap.
   Block *t0 = Memory::DebugGetGlobalTrackingHead();
   
   CHECK( t0 != 0 );
   CHECK( t0->GetAllocIndex() == 4);
   CHECK( strcmp(t0->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t0->GetLineNum() == 550);
   CHECK( t0->GetGlobalPrev() == 0 );
   CHECK( t0->GetGlobalNext() != 0 );

   Block *t1 = t0->GetGlobalNext();

   CHECK( t1 != 0 );
   CHECK( t1->GetAllocIndex() == 3 );
   CHECK( strcmp(t1->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t1->GetLineNum() == 549);
   CHECK( t1->GetGlobalPrev() == t0 );
   CHECK( t1->GetGlobalNext() != 0 );
   CHECK( t0->GetGlobalNext() == t1 );

   Block *t2 = t1->GetGlobalNext();

   CHECK( t2 != 0 );
   CHECK( t2->GetAllocIndex() == 2 );
   CHECK( strcmp(t2->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t2->GetLineNum() == 548);
   CHECK( t2->GetGlobalPrev() == t1 );
   CHECK( t2->GetGlobalNext() != 0 );
   CHECK( t1->GetGlobalNext() == t2 );

   Block *t3 = t2->GetGlobalNext();

   CHECK( t3 != 0 );
   CHECK( t3->GetAllocIndex() == 1 );
   CHECK( strcmp(t3->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t3->GetLineNum() == 547);
   CHECK( t3->GetGlobalPrev() == t2 );
   CHECK( t3->GetGlobalNext() == 0 );
   CHECK( t2->GetGlobalNext() == t3 );
#endif // _DEBUG

   // Now remove the last one --------------------------------------------
   
   delete p2;

   // Yes it works.
	Memory::GetInfo( mem_Info );

   CHECK( mem_Info.CurrBytesUsed == 3*sizeof(Dog) );
   CHECK( mem_Info.CurrHeapCount == 1 );
   CHECK( mem_Info.CurrNumAlloc == 3 );
   CHECK( mem_Info.PeakBytesUsed == 4*sizeof(Dog) );
   CHECK( mem_Info.PeakHeapCount == 1 );
   CHECK( mem_Info.PeakNumAlloc == 4 );

#ifdef _DEBUG
   // OK, now all 4 allocations are created 1 deleted, now look at track info from heap.
   t0 = Memory::DebugGetGlobalTrackingHead();
   
   CHECK( t0 != 0 );
   CHECK( t0->GetAllocIndex() == 4 );
   CHECK( strcmp(t0->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t0->GetLineNum() == 550);
   CHECK( t0->GetGlobalPrev() == 0 );
   CHECK( t0->GetGlobalNext() != 0 );

   t1 = t0->GetGlobalNext();

   CHECK( t1 != 0 );
   CHECK( t1->GetAllocIndex() == 2 );
   CHECK( strcmp(t1->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t1->GetLineNum() == 548);
   CHECK( t1->GetGlobalPrev() == t0 );
   CHECK( t1->GetGlobalNext() != 0 );
   CHECK( t0->GetGlobalNext() == t1 );

   t2 = t1->GetGlobalNext();

   CHECK( t2 != 0 );
   CHECK( t2->GetAllocIndex() == 1 );
   CHECK( strcmp(t2->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t2->GetLineNum() == 547);
   CHECK( t2->GetGlobalPrev() == t1 );
   CHECK( t2->GetGlobalNext() == 0 );
   CHECK( t1->GetGlobalNext() == t2 );

   t3 = t2->GetGlobalNext();

   CHECK( t3 == 0 );
#endif // _DEBUG

   delete p0;
   delete p1;
   delete p3;
}


TEST( Add4Allocs_RemoveAll_TrackingThroughHeap, Tracking_Global_group)
{
	   // Initialize the memory system
	Memory::Initialize();

	// Test the mem Info structures
	Memory::Info mem_Info;

	// Yes it works.
	Memory::GetInfo( mem_Info );

   // validate it
   CHECK( mem_Info.CurrBytesUsed == 0 );
   CHECK( mem_Info.CurrHeapCount == 0 );
   CHECK( mem_Info.CurrNumAlloc == 0 );
   CHECK( mem_Info.PeakBytesUsed == 0 );
   CHECK( mem_Info.PeakHeapCount == 0 );
   CHECK( mem_Info.PeakNumAlloc == 0 );

	// Create a heap pointer (someday make it a handle)
	Heap *heap_A;

	// create the heap by using the memory system
	Memory::Code memResult;
	memResult = Memory::HeapCreate(heap_A, 2*1024*1024, "HEAP_A" );
	CHECK( memResult == Memory::OK );

   // make sure it Initialized to something
   CHECK( heap_A != 0 );

	// Test the heap Info structures
	Heap::Info heapA_Info;

	// Yes it works.
	heap_A->GetInfo( heapA_Info );

   CHECK( heapA_Info.CurrBytesUsed == 0 );
   CHECK( heapA_Info.CurrNumAlloc == 0 );
   CHECK( heapA_Info.EndAddr == 2*1024*1024 + (unsigned int)heap_A );
   CHECK( strcmp( heapA_Info.Name,"HEAP_A") == 0 );
   CHECK( heapA_Info.StartAddr == (unsigned int)heap_A );
   CHECK( heapA_Info.PeakBytesUsed == 0 );
   CHECK( heapA_Info.PeakNumAlloc == 0 );
   CHECK( heapA_Info.TotalSize == 2*1024*1024 );

	// Yes it works.
	Memory::GetInfo( mem_Info );

   CHECK( mem_Info.CurrBytesUsed == 0 );
   CHECK( mem_Info.CurrHeapCount == 1 );
   CHECK( mem_Info.CurrNumAlloc == 0 );
   CHECK( mem_Info.PeakBytesUsed == 0 );
   CHECK( mem_Info.PeakHeapCount == 1 );
   CHECK( mem_Info.PeakNumAlloc == 0 );

   // OK at this point, we have a heap create, so let's allocate from it.

   Dog *p0 = new(heap_A,Memory::Align::Byte_4,__FILE__,__LINE__) Dog();
   Dog *p1 = new(heap_A,Memory::Align::Byte_4,__FILE__,__LINE__) Dog();
   Dog *p2 = new(heap_A,Memory::Align::Byte_4,__FILE__,__LINE__) Dog();
   Dog *p3 = new(heap_A,Memory::Align::Byte_4,__FILE__,__LINE__) Dog();

   	// Yes it works.
	Memory::GetInfo( mem_Info );

   CHECK( mem_Info.CurrBytesUsed == 4*sizeof(Dog) );
   CHECK( mem_Info.CurrHeapCount == 1 );
   CHECK( mem_Info.CurrNumAlloc == 4 );
   CHECK( mem_Info.PeakBytesUsed == 4*sizeof(Dog) );
   CHECK( mem_Info.PeakHeapCount == 1 );
   CHECK( mem_Info.PeakNumAlloc == 4 );

#ifdef _DEBUG
   // OK, now all 4 allocations are created, now look at track info from heap.
   Block *t0 = Memory::DebugGetGlobalTrackingHead();
   
   CHECK( t0 != 0 );
   CHECK( t0->GetAllocIndex() == 4 );
   CHECK( strcmp(t0->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t0->GetLineNum() == 720);
   CHECK( t0->GetGlobalPrev() == 0 );
   CHECK( t0->GetGlobalNext() != 0 );

   Block *t1 = t0->GetGlobalNext();

   CHECK( t1 != 0 );
   CHECK( t1->GetAllocIndex() == 3 );
   CHECK( strcmp(t1->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t1->GetLineNum() == 719);
   CHECK( t1->GetGlobalPrev() == t0 );
   CHECK( t1->GetGlobalNext() != 0 );
   CHECK( t0->GetGlobalNext() == t1 );

   Block *t2 = t1->GetGlobalNext();

   CHECK( t2 != 0 );
   CHECK( t2->GetAllocIndex() == 2 );
   CHECK( strcmp(t2->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t2->GetLineNum() == 718);
   CHECK( t2->GetGlobalPrev() == t1 );
   CHECK( t2->GetGlobalNext() != 0 );
   CHECK( t1->GetGlobalNext() == t2 );

   Block *t3 = t2->GetGlobalNext();

   CHECK( t3 != 0 );
   CHECK( t3->GetAllocIndex() == 1 );
   CHECK( strcmp(t3->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t3->GetLineNum() == 717);
   CHECK( t3->GetGlobalPrev() == t2 );
   CHECK( t3->GetGlobalNext() == 0 );
   CHECK( t2->GetGlobalNext() == t3 );
#endif // _DEBUG

   // Now remove the last one --------------------------------------------
   
   delete p0;

   // Yes it works.
	Memory::GetInfo( mem_Info );

   CHECK( mem_Info.CurrBytesUsed == 3*sizeof(Dog) );
   CHECK( mem_Info.CurrHeapCount == 1 );
   CHECK( mem_Info.CurrNumAlloc == 3 );
   CHECK( mem_Info.PeakBytesUsed == 4*sizeof(Dog) );
   CHECK( mem_Info.PeakHeapCount == 1 );
   CHECK( mem_Info.PeakNumAlloc == 4 );

#ifdef _DEBUG
   // OK, now all 4 allocations are created 1 deleted, now look at track info from heap.
   t0 = Memory::DebugGetGlobalTrackingHead();
   
   CHECK( t0 != 0 );
   CHECK( t0->GetAllocIndex() == 4 );
   CHECK( strcmp(t0->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t0->GetLineNum() == 720);
   CHECK( t0->GetGlobalPrev() == 0 );
   CHECK( t0->GetGlobalNext() != 0 );

   t1 = t0->GetGlobalNext();

   CHECK( t1 != 0 );
   CHECK( t1->GetAllocIndex() == 3 );
   CHECK( strcmp(t1->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t1->GetLineNum() == 719);
   CHECK( t1->GetGlobalPrev() == t0 );
   CHECK( t1->GetGlobalNext() != 0 );
   CHECK( t0->GetGlobalNext() == t1 );

   t2 = t1->GetGlobalNext();

   CHECK( t2 != 0 );
   CHECK( t2->GetAllocIndex() == 2 );
   CHECK( strcmp(t2->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t2->GetLineNum() == 718);
   CHECK( t2->GetGlobalPrev() == t1 );
   CHECK( t2->GetGlobalNext() == 0 );
   CHECK( t1->GetGlobalNext() == t2 );

   t3 = t2->GetGlobalNext();

   CHECK( t3 == 0 );
#endif // _DEBUG

   // Now remove the last one --------------------------------------------
   
   delete p1;

   // Yes it works.
	Memory::GetInfo( mem_Info );

   CHECK( mem_Info.CurrBytesUsed == 2*sizeof(Dog) );
   CHECK( mem_Info.CurrHeapCount == 1 );
   CHECK( mem_Info.CurrNumAlloc == 2 );
   CHECK( mem_Info.PeakBytesUsed == 4*sizeof(Dog) );
   CHECK( mem_Info.PeakHeapCount == 1 );
   CHECK( mem_Info.PeakNumAlloc == 4 );

#ifdef _DEBUG
   // OK, now all 4 allocations are created 1 deleted, now look at track info from heap.
   t0 = Memory::DebugGetGlobalTrackingHead();
   
   CHECK( t0 != 0 );
   CHECK( t0->GetAllocIndex() == 4);
   CHECK( strcmp(t0->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t0->GetLineNum() == 720);
   CHECK( t0->GetGlobalPrev() == 0 );
   CHECK( t0->GetGlobalNext() != 0 );

   t1 = t0->GetGlobalNext();

   CHECK( t1 != 0 );
   CHECK( t1->GetAllocIndex() == 3 );
   CHECK( strcmp(t1->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t1->GetLineNum() == 719);
   CHECK( t1->GetGlobalPrev() == t0 );
   CHECK( t1->GetGlobalNext() == 0 );
   CHECK( t0->GetGlobalNext() == t1 );

   t2 = t1->GetGlobalNext();

   CHECK( t2 == 0 );
#endif // _DEBUG

   // Now remove the last one --------------------------------------------
   
   delete p2;

   // Yes it works.
	Memory::GetInfo( mem_Info );

   CHECK( mem_Info.CurrBytesUsed == 1*sizeof(Dog) );
   CHECK( mem_Info.CurrHeapCount == 1 );
   CHECK( mem_Info.CurrNumAlloc == 1 );
   CHECK( mem_Info.PeakBytesUsed == 4*sizeof(Dog) );
   CHECK( mem_Info.PeakHeapCount == 1 );
   CHECK( mem_Info.PeakNumAlloc == 4 );

#ifdef _DEBUG
   // OK, now all 4 allocations are created 1 deleted, now look at track info from heap.
   t0 = Memory::DebugGetGlobalTrackingHead();
   
   CHECK( t0 != 0 );
   CHECK( t0->GetAllocIndex() == 4 );
   CHECK( strcmp(t0->GetFileName(), "trackingglobal_tests.cpp") == 0);
   CHECK( t0->GetLineNum() == 720);
   CHECK( t0->GetGlobalPrev() == 0 );
   CHECK( t0->GetGlobalNext() == 0 );

   t1 = t0->GetGlobalNext();

   CHECK( t1 == 0 );
#endif // _DEBUG

   // Now remove the last one --------------------------------------------
   
   delete p3;

   // Yes it works.
	Memory::GetInfo( mem_Info );

   CHECK( mem_Info.CurrBytesUsed == 0 );
   CHECK( mem_Info.CurrHeapCount == 1 );
   CHECK( mem_Info.CurrNumAlloc == 0 );
   CHECK( mem_Info.PeakBytesUsed == 4*sizeof(Dog) );
   CHECK( mem_Info.PeakHeapCount == 1 );
   CHECK( mem_Info.PeakNumAlloc == 4 );

#ifdef _DEBUG
   // OK, now all 4 allocations are created 1 deleted, now look at track info from heap.
   t0 = Memory::DebugGetGlobalTrackingHead();
   
   CHECK( t0 == 0 );
#endif // _DEBUG
}

// ---  End of File ---------------
