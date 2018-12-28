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

TEST( Add5Allocs_TrackingThroughHeap, Tracking_group)
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

	// start checks
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
   Block *t0 = heap_A->GetTrackingHead();
   
   CHECK( t0 != 0 );
   CHECK( t0->GetAllocIndex() == 5 );
   CHECK( strcmp(t0->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t0->GetLineNum() == 77);
   CHECK( t0->GetHeapPrev() == 0 );
   CHECK( t0->GetHeapNext() != 0 );

   Block *t1 = t0->GetHeapNext();

   CHECK( t1 != 0 );
   CHECK( t1->GetAllocIndex() == 4 );
   CHECK( strcmp(t1->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t1->GetLineNum() == 76);
   CHECK( t1->GetHeapPrev() == t0 );
   CHECK( t1->GetHeapNext() != 0 );
   CHECK( t0->GetHeapNext() == t1 );

   Block *t2 = t1->GetHeapNext();

   CHECK( t2 != 0 );
   CHECK( t2->GetAllocIndex() ==3 );
   CHECK( strcmp(t2->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t2->GetLineNum() == 75);
   CHECK( t2->GetHeapPrev() == t1 );
   CHECK( t2->GetHeapNext() != 0 );
   CHECK( t1->GetHeapNext() == t2 );

   Block *t3 = t2->GetHeapNext();

   CHECK( t3 != 0 );
   CHECK( t3->GetAllocIndex() == 2 );
   CHECK( strcmp(t3->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t3->GetLineNum() == 74);
   CHECK( t3->GetHeapPrev() == t2 );
   CHECK( t3->GetHeapNext() != 0 );
   CHECK( t2->GetHeapNext() == t3 );

   Block *t4 = t3->GetHeapNext();

   CHECK( t4 != 0 );
   CHECK( t4->GetAllocIndex() == 1 );
   CHECK( strcmp(t4->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t4->GetLineNum() == 73);
   CHECK( t4->GetHeapPrev() == t3 );
   CHECK( t4->GetHeapNext() == 0 );
   CHECK( t3->GetHeapNext() == t4 );
#endif // _DEBUG

   delete p0;
   delete p1;
   delete p2;
   delete p3;
   delete p4;
}



TEST( Add4Allocs_RemoveLast_TrackingThroughHeap, Tracking_group)
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
   Block *t0 = heap_A->GetTrackingHead();
   
   CHECK( t0 != 0 );
   CHECK( t0->GetAllocIndex() == 4 );
   CHECK( strcmp(t0->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t0->GetLineNum() == 210);
   CHECK( t0->GetHeapPrev() == 0 );
   CHECK( t0->GetHeapNext() != 0 );

   Block *t1 = t0->GetHeapNext();

   CHECK( t1 != 0 );
   CHECK( t1->GetAllocIndex() == 3 );
   CHECK( strcmp(t1->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t1->GetLineNum() == 209);
   CHECK( t1->GetHeapPrev() == t0 );
   CHECK( t1->GetHeapNext() != 0 );
   CHECK( t0->GetHeapNext() == t1 );

   Block *t2 = t1->GetHeapNext();

   CHECK( t2 != 0 );
   CHECK( t2->GetAllocIndex() == 2 );
   CHECK( strcmp(t2->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t2->GetLineNum() == 208);
   CHECK( t2->GetHeapPrev() == t1 );
   CHECK( t2->GetHeapNext() != 0 );
   CHECK( t1->GetHeapNext() == t2 );

   Block *t3 = t2->GetHeapNext();

   CHECK( t3 != 0 );
   CHECK( t3->GetAllocIndex() == 1 );
   CHECK( strcmp(t3->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t3->GetLineNum() == 207);
   CHECK( t3->GetHeapPrev() == t2 );
   CHECK( t3->GetHeapNext() == 0 );
   CHECK( t2->GetHeapNext() == t3 );
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
   t0 = heap_A->GetTrackingHead();
   
   CHECK( t0 != 0 );
   CHECK( t0->GetAllocIndex() == 4 );
   CHECK( strcmp(t0->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t0->GetLineNum() == 210);
   CHECK( t0->GetHeapPrev() == 0 );
   CHECK( t0->GetHeapNext() != 0 );

   t1 = t0->GetHeapNext();

   CHECK( t1 != 0 );
   CHECK( t1->GetAllocIndex() == 3 );
   CHECK( strcmp(t1->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t1->GetLineNum() == 209);
   CHECK( t1->GetHeapPrev() == t0 );
   CHECK( t1->GetHeapNext() != 0 );
   CHECK( t0->GetHeapNext() == t1 );

   t2 = t1->GetHeapNext();

   CHECK( t2 != 0 );
   CHECK( t2->GetAllocIndex() == 2 );
   CHECK( strcmp(t2->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t2->GetLineNum() == 208);
   CHECK( t2->GetHeapPrev() == t1 );
   CHECK( t2->GetHeapNext() == 0 );
   CHECK( t1->GetHeapNext() == t2 );

   t3 = t2->GetHeapNext();

   CHECK( t3 == 0 );
#endif // _DEBUG

   delete p1;
   delete p2;
   delete p3;
}


TEST( Add4Allocs_RemoveFirst_TrackingThroughHeap, Tracking_group)
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
   Block *t0 = heap_A->GetTrackingHead();
   
   CHECK( t0 != 0 );
   CHECK( t0->GetAllocIndex() == 4);
   CHECK( strcmp(t0->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t0->GetLineNum() == 380);
   CHECK( t0->GetHeapPrev() == 0 );
   CHECK( t0->GetHeapNext() != 0 );

   Block *t1 = t0->GetHeapNext();

   CHECK( t1 != 0 );
   CHECK( t1->GetAllocIndex() == 3 );
   CHECK( strcmp(t1->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t1->GetLineNum() == 379);
   CHECK( t1->GetHeapPrev() == t0 );
   CHECK( t1->GetHeapNext() != 0 );
   CHECK( t0->GetHeapNext() == t1 );

   Block *t2 = t1->GetHeapNext();

   CHECK( t2 != 0 );
   CHECK( t2->GetAllocIndex() ==2 );
   CHECK( strcmp(t2->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t2->GetLineNum() == 378);
   CHECK( t2->GetHeapPrev() == t1 );
   CHECK( t2->GetHeapNext() != 0 );
   CHECK( t1->GetHeapNext() == t2 );

   Block *t3 = t2->GetHeapNext();

   CHECK( t3 != 0 );
   CHECK( t3->GetAllocIndex() == 1 );
   CHECK( strcmp(t3->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t3->GetLineNum() == 377);
   CHECK( t3->GetHeapPrev() == t2 );
   CHECK( t3->GetHeapNext() == 0 );
   CHECK( t2->GetHeapNext() == t3 );
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
   t0 = heap_A->GetTrackingHead();
   
   CHECK( t0 != 0 );
   CHECK( t0->GetAllocIndex() == 3);
   CHECK( strcmp(t0->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t0->GetLineNum() == 379);
   CHECK( t0->GetHeapPrev() == 0 );
   CHECK( t0->GetHeapNext() != 0 );

   t1 = t0->GetHeapNext();

   CHECK( t1 != 0 );
   CHECK( t1->GetAllocIndex() == 2 );
   CHECK( strcmp(t1->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t1->GetLineNum() == 378);
   CHECK( t1->GetHeapPrev() == t0 );
   CHECK( t1->GetHeapNext() != 0 );
   CHECK( t0->GetHeapNext() == t1 );

   t2 = t1->GetHeapNext();

   CHECK( t2 != 0 );
   CHECK( t2->GetAllocIndex() == 1 );
   CHECK( strcmp(t2->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t2->GetLineNum() == 377);
   CHECK( t2->GetHeapPrev() == t1 );
   CHECK( t2->GetHeapNext() == 0 );
   CHECK( t1->GetHeapNext() == t2 );

   t3 = t2->GetHeapNext();

   CHECK( t3 == 0 );
#endif // _DEBUG

   delete p0;
   delete p1;
   delete p2;
}


TEST( Add4Allocs_RemoveMiddle_TrackingThroughHeap, Tracking_group)
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
   Block *t0 = heap_A->GetTrackingHead();
   
   CHECK( t0 != 0 );
   CHECK( t0->GetAllocIndex() == 4 );
   CHECK( strcmp(t0->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t0->GetLineNum() == 550);
   CHECK( t0->GetHeapPrev() == 0 );
   CHECK( t0->GetHeapNext() != 0 );

   Block *t1 = t0->GetHeapNext();

   CHECK( t1 != 0 );
   CHECK( t1->GetAllocIndex() == 3 );
   CHECK( strcmp(t1->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t1->GetLineNum() == 549);
   CHECK( t1->GetHeapPrev() == t0 );
   CHECK( t1->GetHeapNext() != 0 );
   CHECK( t0->GetHeapNext() == t1 );

   Block *t2 = t1->GetHeapNext();

   CHECK( t2 != 0 );
   CHECK( t2->GetAllocIndex() == 2 );
   CHECK( strcmp(t2->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t2->GetLineNum() == 548);
   CHECK( t2->GetHeapPrev() == t1 );
   CHECK( t2->GetHeapNext() != 0 );
   CHECK( t1->GetHeapNext() == t2 );

   Block *t3 = t2->GetHeapNext();

   CHECK( t3 != 0 );
   CHECK( t3->GetAllocIndex() == 1 );
   CHECK( strcmp(t3->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t3->GetLineNum() == 547);
   CHECK( t3->GetHeapPrev() == t2 );
   CHECK( t3->GetHeapNext() == 0 );
   CHECK( t2->GetHeapNext() == t3 );
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
   t0 = heap_A->GetTrackingHead();
   
   CHECK( t0 != 0 );
   CHECK( t0->GetAllocIndex() == 4 );
   CHECK( strcmp(t0->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t0->GetLineNum() == 550);
   CHECK( t0->GetHeapPrev() == 0 );
   CHECK( t0->GetHeapNext() != 0 );

   t1 = t0->GetHeapNext();

   CHECK( t1 != 0 );
   CHECK( t1->GetAllocIndex() == 2 );
   CHECK( strcmp(t1->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t1->GetLineNum() == 548);
   CHECK( t1->GetHeapPrev() == t0 );
   CHECK( t1->GetHeapNext() != 0 );
   CHECK( t0->GetHeapNext() == t1 );

   t2 = t1->GetHeapNext();

   CHECK( t2 != 0 );
   CHECK( t2->GetAllocIndex() == 1 );
   CHECK( strcmp(t2->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t2->GetLineNum() == 547);
   CHECK( t2->GetHeapPrev() == t1 );
   CHECK( t2->GetHeapNext() == 0 );
   CHECK( t1->GetHeapNext() == t2 );

   t3 = t2->GetHeapNext();

   CHECK( t3 == 0 );
#endif // _DEBUG

   delete p0;
   delete p1;
   delete p3;
}


TEST( Add4Allocs_RemoveAll_TrackingThroughHeap, Tracking_group)
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
   Block *t0 = heap_A->GetTrackingHead();
   
   CHECK( t0 != 0 );
   CHECK( t0->GetAllocIndex() == 4 );
   CHECK( strcmp(t0->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t0->GetLineNum() == 720);
   CHECK( t0->GetHeapPrev() == 0 );
   CHECK( t0->GetHeapNext() != 0 );

   Block *t1 = t0->GetHeapNext();

   CHECK( t1 != 0 );
   CHECK( t1->GetAllocIndex() == 3 );
   CHECK( strcmp(t1->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t1->GetLineNum() == 719);
   CHECK( t1->GetHeapPrev() == t0 );
   CHECK( t1->GetHeapNext() != 0 );
   CHECK( t0->GetHeapNext() == t1 );

   Block *t2 = t1->GetHeapNext();

   CHECK( t2 != 0 );
   CHECK( t2->GetAllocIndex() == 2 );
   CHECK( strcmp(t2->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t2->GetLineNum() == 718);
   CHECK( t2->GetHeapPrev() == t1 );
   CHECK( t2->GetHeapNext() != 0 );
   CHECK( t1->GetHeapNext() == t2 );

   Block *t3 = t2->GetHeapNext();

   CHECK( t3 != 0 );
   CHECK( t3->GetAllocIndex() == 1);
   CHECK( strcmp(t3->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t3->GetLineNum() == 717);
   CHECK( t3->GetHeapPrev() == t2 );
   CHECK( t3->GetHeapNext() == 0 );
   CHECK( t2->GetHeapNext() == t3 );
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
   t0 = heap_A->GetTrackingHead();
   
   CHECK( t0 != 0 );
   CHECK( t0->GetAllocIndex() == 4 );
   CHECK( strcmp(t0->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t0->GetLineNum() == 720);
   CHECK( t0->GetHeapPrev() == 0 );
   CHECK( t0->GetHeapNext() != 0 );

   t1 = t0->GetHeapNext();

   CHECK( t1 != 0 );
   CHECK( t1->GetAllocIndex() == 3 );
   CHECK( strcmp(t1->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t1->GetLineNum() == 719);
   CHECK( t1->GetHeapPrev() == t0 );
   CHECK( t1->GetHeapNext() != 0 );
   CHECK( t0->GetHeapNext() == t1 );

   t2 = t1->GetHeapNext();

   CHECK( t2 != 0 );
   CHECK( t2->GetAllocIndex() == 2 );
   CHECK( strcmp(t2->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t2->GetLineNum() == 718);
   CHECK( t2->GetHeapPrev() == t1 );
   CHECK( t2->GetHeapNext() == 0 );
   CHECK( t1->GetHeapNext() == t2 );

   t3 = t2->GetHeapNext();

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
   t0 = heap_A->GetTrackingHead();
   
   CHECK( t0 != 0 );
   CHECK( t0->GetAllocIndex() == 4 );
   CHECK( strcmp(t0->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t0->GetLineNum() == 720);
   CHECK( t0->GetHeapPrev() == 0 );
   CHECK( t0->GetHeapNext() != 0 );

   t1 = t0->GetHeapNext();

   CHECK( t1 != 0 );
   CHECK( t1->GetAllocIndex() == 3 );
   CHECK( strcmp(t1->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t1->GetLineNum() == 719);
   CHECK( t1->GetHeapPrev() == t0 );
   CHECK( t1->GetHeapNext() == 0 );
   CHECK( t0->GetHeapNext() == t1 );

   t2 = t1->GetHeapNext();

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
   t0 = heap_A->GetTrackingHead();
   
   CHECK( t0 != 0 );
   CHECK( t0->GetAllocIndex() == 4 );
   CHECK( strcmp(t0->GetFileName(), "trackingheap_tests.cpp") == 0);
   CHECK( t0->GetLineNum() == 720);
   CHECK( t0->GetHeapPrev() == 0 );
   CHECK( t0->GetHeapNext() == 0 );

   t1 = t0->GetHeapNext();

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
   t0 = heap_A->GetTrackingHead();
   
   CHECK( t0 == 0 );
#endif // _DEBUG
}

// ---  End of File ---------------
