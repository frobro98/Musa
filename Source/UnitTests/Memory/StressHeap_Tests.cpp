//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#include "Framework/UnitTest.h"
#include "Memory/MemoryEngine.h"
#include "Dog.h"

//--------------------------------------------------------------------------
// MACROS Test
//--------------------------------------------------------------------------

#define TEST_4_BYTE(ptr)      ( ((unsigned int)ptr & 0x03) == 0 )
#define TEST_8_BYTE(ptr)      ( ((unsigned int)ptr & 0x07) == 0 )
#define TEST_16_BYTE(ptr)     ( ((unsigned int)ptr & 0x0F) == 0 )
#define TEST_32_BYTE(ptr)     ( ((unsigned int)ptr & 0x1F) == 0 )
#define TEST_64_BYTE(ptr)     ( ((unsigned int)ptr & 0x3F) == 0 )
#define TEST_128_BYTE(ptr)    ( ((unsigned int)ptr & 0x7F) == 0 )
#define TEST_256_BYTE(ptr)    ( ((unsigned int)ptr & 0xFF) == 0 )

//---------------------------------------------------------------------------
// Test Files:
// Single step throught, set break points, but do not modify it
//---------------------------------------------------------------------------

TEST(test1_heapcCheck, stressTestHeap_group)
{
	// Initialize the memory system
	Memory::Initialize();

	// create heap
	Memory::Code memReturn;
	Heap *heap_A;
	Heap *heap_B;
	Heap *heap_C;

	memReturn = Memory::HeapCreate(heap_A, 100 * 1024, "HEAP_A");
	CHECK(memReturn == Memory::OK);
	memReturn = Memory::HeapCreate(heap_B, 50 * 1024, "HEAP_B");
	CHECK(memReturn == Memory::OK);
	memReturn = Memory::HeapCreate(heap_C, 500 * 1024, "HEAP_C");
	CHECK(memReturn == Memory::OK);

	char *p0 = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__) char[300];
	char *p1 = new(heap_B, Memory::Align::Byte_4, __FILE__, __LINE__) char[200];
	char *p2 = new(heap_B, Memory::Align::Byte_4, __FILE__, __LINE__) char[5 * 1024];
	char *p3 = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__) char[60 * 1024];
	char *p4 = new(heap_C, Memory::Align::Byte_4, __FILE__, __LINE__) char[200 * 1024];
	char *p5 = new(heap_C, Memory::Align::Byte_4, __FILE__, __LINE__) char[8 * 1024];
	char *p6 = new(heap_B, Memory::Align::Byte_4, __FILE__, __LINE__) char[15 * 1024];
	char *p7 = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__) char[10 * 1024];

	CHECK(TEST_4_BYTE(p0));
	CHECK(TEST_4_BYTE(p1));
	CHECK(TEST_4_BYTE(p2));
	CHECK(TEST_4_BYTE(p3));
	CHECK(TEST_4_BYTE(p4));
	CHECK(TEST_4_BYTE(p5));
	CHECK(TEST_4_BYTE(p6));
	CHECK(TEST_4_BYTE(p7));

	Memory::Info info;
	Memory::GetInfo(info);

	// CHECK the global memory
	CHECK(info.CurrAllocIndex == 8);
	CHECK(info.CurrBytesUsed == (5 + 60 + 200 + 8 + 15 + 10) * 1024 + (200 + 300));
	CHECK(info.CurrHeapCount == 3);
	CHECK(info.CurrNumAlloc == 8);
	CHECK(info.PeakBytesUsed == (5 + 60 + 200 + 8 + 15 + 10) * 1024 + (200 + 300));
	CHECK(info.PeakHeapCount == 3);
	CHECK(info.PeakNumAlloc == 8);

	// CHECK the memory of each HEAP
	Heap::Info HeapInfo;

	heap_A->GetInfo(HeapInfo);
	CHECK(HeapInfo.CurrBytesUsed == (60 + 10) * 1024 + 300);
	CHECK(HeapInfo.CurrNumAlloc == 3);
	CHECK(strcmp(HeapInfo.Name, "HEAP_A") == 0);
	CHECK(HeapInfo.PeakBytesUsed == (60 + 10) * 1024 + 300);
	CHECK(HeapInfo.PeakNumAlloc == 3);
	CHECK(HeapInfo.TotalSize == 100 * 1024);

	heap_B->GetInfo(HeapInfo);
	CHECK(HeapInfo.CurrBytesUsed == (5 + 15) * 1024 + 200);
	CHECK(HeapInfo.CurrNumAlloc == 3);
	CHECK(strcmp(HeapInfo.Name, "HEAP_B") == 0);
	CHECK(HeapInfo.PeakBytesUsed == (5 + 15) * 1024 + 200);
	CHECK(HeapInfo.PeakNumAlloc == 3);
	CHECK(HeapInfo.TotalSize == 50 * 1024);

	heap_C->GetInfo(HeapInfo);
	CHECK(HeapInfo.CurrBytesUsed == (200 + 8) * 1024);
	CHECK(HeapInfo.CurrNumAlloc == 2);
	CHECK(strcmp(HeapInfo.Name, "HEAP_C") == 0);
	CHECK(HeapInfo.PeakBytesUsed == (200 + 8) * 1024);
	CHECK(HeapInfo.PeakNumAlloc == 2);
	CHECK(HeapInfo.TotalSize == 500 * 1024);

	delete p0;
	delete p1;
	delete p2;
	delete p3;
	delete p4;
	delete p5;
	delete p6;
	delete p7;

	Memory::GetInfo(info);

	// CHECK the global memory
	CHECK(info.CurrAllocIndex == 8);
	CHECK(info.CurrBytesUsed == 0);
	CHECK(info.CurrHeapCount == 3);
	CHECK(info.CurrNumAlloc == 0);
	CHECK(info.PeakBytesUsed == (5 + 60 + 200 + 8 + 15 + 10) * 1024 + (200 + 300));
	CHECK(info.PeakHeapCount == 3);
	CHECK(info.PeakNumAlloc == 8);

	// CHECK the memory of each HEAP

	heap_A->GetInfo(HeapInfo);
	CHECK(HeapInfo.CurrBytesUsed == 0);
	CHECK(HeapInfo.CurrNumAlloc == 0);
	CHECK(strcmp(HeapInfo.Name, "HEAP_A") == 0);
	CHECK(HeapInfo.PeakBytesUsed == (60 + 10) * 1024 + 300);
	CHECK(HeapInfo.PeakNumAlloc == 3);
	CHECK(HeapInfo.TotalSize == 100 * 1024);

	heap_B->GetInfo(HeapInfo);
	CHECK(HeapInfo.CurrBytesUsed == 0);
	CHECK(HeapInfo.CurrNumAlloc == 0);
	CHECK(strcmp(HeapInfo.Name, "HEAP_B") == 0);
	CHECK(HeapInfo.PeakBytesUsed == (5 + 15) * 1024 + 200);
	CHECK(HeapInfo.PeakNumAlloc == 3);
	CHECK(HeapInfo.TotalSize == 50 * 1024);

	heap_C->GetInfo(HeapInfo);
	CHECK(HeapInfo.CurrBytesUsed == 0);
	CHECK(HeapInfo.CurrNumAlloc == 0);
	CHECK(strcmp(HeapInfo.Name, "HEAP_C") == 0);
	CHECK(HeapInfo.PeakBytesUsed == (200 + 8) * 1024);
	CHECK(HeapInfo.PeakNumAlloc == 2);
	CHECK(HeapInfo.TotalSize == 500 * 1024);

	Memory::HeapDestroy(heap_A);
	Memory::HeapDestroy(heap_B);
	Memory::HeapDestroy(heap_C);

	Memory::GetInfo(info);

	// CHECK the global memory
	CHECK(info.CurrAllocIndex == 8);
	CHECK(info.CurrBytesUsed == 0);
	CHECK(info.CurrHeapCount == 0);
	CHECK(info.CurrNumAlloc == 0);
	CHECK(info.PeakBytesUsed == (5 + 60 + 200 + 8 + 15 + 10) * 1024 + (200 + 300));
	CHECK(info.PeakHeapCount == 3);
	CHECK(info.PeakNumAlloc == 8);
}

TEST(test1_heapcCheck2, stressTestHeap_group)
{
	// Initialize the memory system
	Memory::Initialize();

	// create heap
	Memory::Code memResult;
	Heap *heap_A;
	memResult = Memory::HeapCreate(heap_A, 100 * 1024, "HEAP_A");
	CHECK(memResult == Memory::OK);
	Heap *heap_B;
	memResult = Memory::HeapCreate(heap_B, 50 * 1024, "HEAP_B");
	CHECK(memResult == Memory::OK);
	Heap *heap_C;
	memResult = Memory::HeapCreate(heap_C, 500 * 1024, "HEAP_C");
	CHECK(memResult == Memory::OK);

	char *p0 = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__) char[300];
	char *p1 = new(heap_B, Memory::Align::Byte_4, __FILE__, __LINE__) char[200];
	char *p2 = new(heap_B, Memory::Align::Byte_4, __FILE__, __LINE__) char[5 * 1024];
	char *p3 = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__) char[60 * 1024];
	char *p4 = new(heap_C, Memory::Align::Byte_4, __FILE__, __LINE__) char[200 * 1024];
	char *p5 = new(heap_C, Memory::Align::Byte_4, __FILE__, __LINE__) char[8 * 1024];
	char *p6 = new(heap_B, Memory::Align::Byte_4, __FILE__, __LINE__) char[15 * 1024];
	char *p7 = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__) char[10 * 1024];

	CHECK(TEST_4_BYTE(p0));
	CHECK(TEST_4_BYTE(p1));
	CHECK(TEST_4_BYTE(p2));
	CHECK(TEST_4_BYTE(p3));
	CHECK(TEST_4_BYTE(p4));
	CHECK(TEST_4_BYTE(p5));
	CHECK(TEST_4_BYTE(p6));
	CHECK(TEST_4_BYTE(p7));

	Memory::Info info;
	Memory::GetInfo(info);

	// CHECK the global memory
	CHECK(info.CurrAllocIndex == 8);
	CHECK(info.CurrBytesUsed == (5 + 60 + 200 + 8 + 15 + 10) * 1024 + (200 + 300));
	CHECK(info.CurrHeapCount == 3);
	CHECK(info.CurrNumAlloc == 8);
	CHECK(info.PeakBytesUsed == (5 + 60 + 200 + 8 + 15 + 10) * 1024 + (200 + 300));
	CHECK(info.PeakHeapCount == 3);
	CHECK(info.PeakNumAlloc == 8);

	// CHECK the memory of each HEAP
	Heap::Info HeapInfo;

	heap_A->GetInfo(HeapInfo);
	CHECK(HeapInfo.CurrBytesUsed == (60 + 10) * 1024 + 300);
	CHECK(HeapInfo.CurrNumAlloc == 3);
	CHECK(strcmp(HeapInfo.Name, "HEAP_A") == 0);
	CHECK(HeapInfo.PeakBytesUsed == (60 + 10) * 1024 + 300);
	CHECK(HeapInfo.PeakNumAlloc == 3);
	CHECK(HeapInfo.TotalSize == 100 * 1024);

	heap_B->GetInfo(HeapInfo);
	CHECK(HeapInfo.CurrBytesUsed == (5 + 15) * 1024 + 200);
	CHECK(HeapInfo.CurrNumAlloc == 3);
	CHECK(strcmp(HeapInfo.Name, "HEAP_B") == 0);
	CHECK(HeapInfo.PeakBytesUsed == (5 + 15) * 1024 + 200);
	CHECK(HeapInfo.PeakNumAlloc == 3);
	CHECK(HeapInfo.TotalSize == 50 * 1024);

	heap_C->GetInfo(HeapInfo);
	CHECK(HeapInfo.CurrBytesUsed == (200 + 8) * 1024);
	CHECK(HeapInfo.CurrNumAlloc == 2);
	CHECK(strcmp(HeapInfo.Name, "HEAP_C") == 0);
	CHECK(HeapInfo.PeakBytesUsed == (200 + 8) * 1024);
	CHECK(HeapInfo.PeakNumAlloc == 2);
	CHECK(HeapInfo.TotalSize == 500 * 1024);

	// delete heap A and it's allocations
	Memory::HeapDestroy(heap_A);

	// CHECK the global memory
	Memory::GetInfo(info);

	CHECK(info.CurrAllocIndex == 8);
	CHECK(info.CurrBytesUsed == (5 + 200 + 8 + 15) * 1024 + (200));
	CHECK(info.CurrHeapCount == 2);
	CHECK(info.CurrNumAlloc == 5);
	CHECK(info.PeakBytesUsed == (5 + 60 + 200 + 8 + 15 + 10) * 1024 + (200 + 300));
	CHECK(info.PeakHeapCount == 3);
	CHECK(info.PeakNumAlloc == 8);


	// delete heap B and it's allocations
	Memory::HeapDestroy(heap_B);

	// CHECK the global memory
	Memory::GetInfo(info);

	CHECK(info.CurrAllocIndex == 8);
	CHECK(info.CurrBytesUsed == (200 + 8) * 1024);
	CHECK(info.CurrHeapCount == 1);
	CHECK(info.CurrNumAlloc == 2);
	CHECK(info.PeakBytesUsed == (5 + 60 + 200 + 8 + 15 + 10) * 1024 + (200 + 300));
	CHECK(info.PeakHeapCount == 3);
	CHECK(info.PeakNumAlloc == 8);

	// delete heap C and it's allocations
	Memory::HeapDestroy(heap_C);

	// CHECK the global memory
	Memory::GetInfo(info);

	CHECK(info.CurrAllocIndex == 8);
	CHECK(info.CurrBytesUsed == 0);
	CHECK(info.CurrHeapCount == 0);
	CHECK(info.CurrNumAlloc == 0);
	CHECK(info.PeakBytesUsed == (5 + 60 + 200 + 8 + 15 + 10) * 1024 + (200 + 300));
	CHECK(info.PeakHeapCount == 3);
	CHECK(info.PeakNumAlloc == 8);
}

TEST(test1_heapcCheck3, stressTestHeap_group)
{
	// Initialize the memory system
	Memory::Initialize();

	// create heap
	Memory::Code memResult;

	Heap *heap_A;
	memResult = Memory::HeapCreate(heap_A, 100 * 1024, "HEAP_A");
	CHECK(memResult == Memory::OK);
	Heap *heap_B;
	memResult = Memory::HeapCreate(heap_B, 50 * 1024, "HEAP_B");
	CHECK(memResult == Memory::OK);
	Heap *heap_C;
	memResult = Memory::HeapCreate(heap_C, 500 * 1024, "HEAP_C");
	CHECK(memResult == Memory::OK);

	char *p0 = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__) char[300];
	char *p1 = new(heap_B, Memory::Align::Byte_4, __FILE__, __LINE__) char[200];
	char *p2 = new(heap_B, Memory::Align::Byte_4, __FILE__, __LINE__) char[5 * 1024];
	char *p3 = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__) char[60 * 1024];
	char *p4 = new(heap_C, Memory::Align::Byte_4, __FILE__, __LINE__) char[200 * 1024];
	char *p5 = new(heap_C, Memory::Align::Byte_4, __FILE__, __LINE__) char[8 * 1024];
	char *p6 = new(heap_B, Memory::Align::Byte_4, __FILE__, __LINE__) char[15 * 1024];
	char *p7 = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__) char[10 * 1024];

	CHECK(TEST_4_BYTE(p0));
	CHECK(TEST_4_BYTE(p1));
	CHECK(TEST_4_BYTE(p2));
	CHECK(TEST_4_BYTE(p3));
	CHECK(TEST_4_BYTE(p4));
	CHECK(TEST_4_BYTE(p5));
	CHECK(TEST_4_BYTE(p6));
	CHECK(TEST_4_BYTE(p7));

	Memory::Info info;
	Memory::GetInfo(info);

	// CHECK the global memory
	CHECK(info.CurrAllocIndex == 8);
	CHECK(info.CurrBytesUsed == (5 + 60 + 200 + 8 + 15 + 10) * 1024 + (200 + 300));
	CHECK(info.CurrHeapCount == 3);
	CHECK(info.CurrNumAlloc == 8);
	CHECK(info.PeakBytesUsed == (5 + 60 + 200 + 8 + 15 + 10) * 1024 + (200 + 300));
	CHECK(info.PeakHeapCount == 3);
	CHECK(info.PeakNumAlloc == 8);

	// CHECK the memory of each HEAP
	Heap::Info HeapInfo;

	heap_A->GetInfo(HeapInfo);
	CHECK(HeapInfo.CurrBytesUsed == (60 + 10) * 1024 + 300);
	CHECK(HeapInfo.CurrNumAlloc == 3);
	CHECK(strcmp(HeapInfo.Name, "HEAP_A") == 0);
	CHECK(HeapInfo.PeakBytesUsed == (60 + 10) * 1024 + 300);
	CHECK(HeapInfo.PeakNumAlloc == 3);
	CHECK(HeapInfo.TotalSize == 100 * 1024);

	heap_B->GetInfo(HeapInfo);
	CHECK(HeapInfo.CurrBytesUsed == (5 + 15) * 1024 + 200);
	CHECK(HeapInfo.CurrNumAlloc == 3);
	CHECK(strcmp(HeapInfo.Name, "HEAP_B") == 0);
	CHECK(HeapInfo.PeakBytesUsed == (5 + 15) * 1024 + 200);
	CHECK(HeapInfo.PeakNumAlloc == 3);
	CHECK(HeapInfo.TotalSize == 50 * 1024);

	heap_C->GetInfo(HeapInfo);
	CHECK(HeapInfo.CurrBytesUsed == (200 + 8) * 1024);
	CHECK(HeapInfo.CurrNumAlloc == 2);
	CHECK(strcmp(HeapInfo.Name, "HEAP_C") == 0);
	CHECK(HeapInfo.PeakBytesUsed == (200 + 8) * 1024);
	CHECK(HeapInfo.PeakNumAlloc == 2);
	CHECK(HeapInfo.TotalSize == 500 * 1024);

	delete p0;
	delete p3;
	delete p7;
	delete p1;
	delete p4;

	Memory::GetInfo(info);

	// CHECK the global memory
	CHECK(info.CurrAllocIndex == 8);
	CHECK(info.CurrBytesUsed == (15 + 5 + 8) * 1024);
	CHECK(info.CurrHeapCount == 3);
	CHECK(info.CurrNumAlloc == 3);
	CHECK(info.PeakBytesUsed == (5 + 60 + 200 + 8 + 15 + 10) * 1024 + (200 + 300));
	CHECK(info.PeakHeapCount == 3);
	CHECK(info.PeakNumAlloc == 8);

	heap_A->GetInfo(HeapInfo);
	CHECK(HeapInfo.CurrBytesUsed == 0);
	CHECK(HeapInfo.CurrNumAlloc == 0);
	CHECK(strcmp(HeapInfo.Name, "HEAP_A") == 0);
	CHECK(HeapInfo.PeakBytesUsed == (60 + 10) * 1024 + 300);
	CHECK(HeapInfo.PeakNumAlloc == 3);
	CHECK(HeapInfo.TotalSize == 100 * 1024);

	heap_B->GetInfo(HeapInfo);
	CHECK(HeapInfo.CurrBytesUsed == (15 + 5) * 1024);
	CHECK(HeapInfo.CurrNumAlloc == 2);
	CHECK(strcmp(HeapInfo.Name, "HEAP_B") == 0);
	CHECK(HeapInfo.PeakBytesUsed == (5 + 15) * 1024 + 200);
	CHECK(HeapInfo.PeakNumAlloc == 3);
	CHECK(HeapInfo.TotalSize == 50 * 1024);

	heap_C->GetInfo(HeapInfo);
	CHECK(HeapInfo.CurrBytesUsed == 8 * 1024);
	CHECK(HeapInfo.CurrNumAlloc == 1);
	CHECK(strcmp(HeapInfo.Name, "HEAP_C") == 0);
	CHECK(HeapInfo.PeakBytesUsed == (200 + 8) * 1024);
	CHECK(HeapInfo.PeakNumAlloc == 2);
	CHECK(HeapInfo.TotalSize == 500 * 1024);

	Memory::HeapDestroy(heap_A);
	Memory::HeapDestroy(heap_B);
	Memory::HeapDestroy(heap_C);

	Memory::GetInfo(info);

	// CHECK the global memory
	CHECK(info.CurrAllocIndex == 8);
	CHECK(info.CurrBytesUsed == 0);
	CHECK(info.CurrHeapCount == 0);
	CHECK(info.CurrNumAlloc == 0);
	CHECK(info.PeakBytesUsed == (5 + 60 + 200 + 8 + 15 + 10) * 1024 + (200 + 300));
	CHECK(info.PeakHeapCount == 3);
	CHECK(info.PeakNumAlloc == 8);
}

// ---  End of File ---------------
