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

TEST(allocate_from_heap, heap_group)
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

	// OK at this point, we have a heap create, so let's allocate from it.

	Dog *p = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__) Dog();

	// Yes it works.
	Memory::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == sizeof(Dog));
	CHECK(mem_Info.CurrHeapCount == 1);
	CHECK(mem_Info.CurrNumAlloc == 1);
	CHECK(mem_Info.PeakBytesUsed == sizeof(Dog));
	CHECK(mem_Info.PeakHeapCount == 1);
	CHECK(mem_Info.PeakNumAlloc == 1);

	delete p;

	Memory::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 1);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == sizeof(Dog));
	CHECK(mem_Info.PeakHeapCount == 1);
	CHECK(mem_Info.PeakNumAlloc == 1);

	Memory::Destroy();
}

TEST(allocate_from_heapARRAY, heap_group)
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

	// OK at this point, we have a heap create, so let's allocate from it.

	Dog *p = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__) Dog[3];

	// Yes it works.
	Memory::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 3 * sizeof(Dog) + 4);
	CHECK(mem_Info.CurrHeapCount == 1);
	CHECK(mem_Info.CurrNumAlloc == 1);
	CHECK(mem_Info.PeakBytesUsed == 3 * sizeof(Dog) + 4);
	CHECK(mem_Info.PeakHeapCount == 1);
	CHECK(mem_Info.PeakNumAlloc == 1);

	delete[] p;

	Memory::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 1);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 3 * sizeof(Dog) + 4);
	CHECK(mem_Info.PeakHeapCount == 1);
	CHECK(mem_Info.PeakNumAlloc == 1);

	Memory::Destroy();
}

TEST(allocate_from_heapARRAY2, heap_group)
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

	// OK at this point, we have a heap create, so let's allocate from it.

	Dog *p0 = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__) Dog[3];
	Dog *p1 = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__) Dog[3];
	Dog *p2 = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__) Dog[3];

	// Yes it works.
	Memory::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 3 * 3 * sizeof(Dog) + 3 * 4);
	CHECK(mem_Info.CurrHeapCount == 1);
	CHECK(mem_Info.CurrNumAlloc == 3);
	CHECK(mem_Info.PeakBytesUsed == 3 * 3 * sizeof(Dog) + 3 * 4);
	CHECK(mem_Info.PeakHeapCount == 1);
	CHECK(mem_Info.PeakNumAlloc == 3);

	delete[] p0;
	delete[] p1;
	delete[] p2;

	Memory::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 1);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 3 * 3 * sizeof(Dog) + 3 * 4);
	CHECK(mem_Info.PeakHeapCount == 1);
	CHECK(mem_Info.PeakNumAlloc == 3);

	Memory::Destroy();
}

TEST(allocate_from_heapARRAY3, heap_group)
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

	// OK at this point, we have a heap create, so let's allocate from it.

	Dog *p0 = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__) Dog[3];
	Dog *p1 = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__) Dog[30];
	Dog *p2 = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__) Dog[50];

	// Yes it works.
	Memory::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == (3 + 30 + 50) * sizeof(Dog) + 3 * 4);
	CHECK(mem_Info.CurrHeapCount == 1);
	CHECK(mem_Info.CurrNumAlloc == 3);
	CHECK(mem_Info.PeakBytesUsed == (3 + 30 + 50) * sizeof(Dog) + 3 * 4);
	CHECK(mem_Info.PeakHeapCount == 1);
	CHECK(mem_Info.PeakNumAlloc == 3);

	delete[] p1;

	Memory::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == (3 + 50) * sizeof(Dog) + 2 * 4);
	CHECK(mem_Info.CurrHeapCount == 1);
	CHECK(mem_Info.CurrNumAlloc == 2);
	CHECK(mem_Info.PeakBytesUsed == (3 + 30 + 50) * sizeof(Dog) + 3 * 4);
	CHECK(mem_Info.PeakHeapCount == 1);
	CHECK(mem_Info.PeakNumAlloc == 3);

	delete[] p0;
	delete[] p2;

	Memory::Destroy();
}

TEST(allocate_5_from_heap, heap_group)
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

	// OK at this point, we have a heap create, so let's allocate from it.

	Dog *p0 = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__) Dog();
	Dog *p1 = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__) Dog();
	Dog *p2 = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__) Dog();
	Dog *p3 = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__) Dog();
	Dog *p4 = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__) Dog();

	// Yes it works.
	Memory::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 5 * sizeof(Dog));
	CHECK(mem_Info.CurrHeapCount == 1);
	CHECK(mem_Info.CurrNumAlloc == 5);
	CHECK(mem_Info.PeakBytesUsed == 5 * sizeof(Dog));
	CHECK(mem_Info.PeakHeapCount == 1);
	CHECK(mem_Info.PeakNumAlloc == 5);

	delete p0;
	delete p3;
	delete p2;
	delete p1;
	delete p4;

	Memory::GetInfo(mem_Info);

	CHECK(mem_Info.CurrBytesUsed == 0);
	CHECK(mem_Info.CurrHeapCount == 1);
	CHECK(mem_Info.CurrNumAlloc == 0);
	CHECK(mem_Info.PeakBytesUsed == 5 * sizeof(Dog));
	CHECK(mem_Info.PeakHeapCount == 1);
	CHECK(mem_Info.PeakNumAlloc == 5);

	Memory::Destroy();
}

// ---  End of File ---------------
