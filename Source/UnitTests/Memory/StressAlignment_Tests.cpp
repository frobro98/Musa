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

TEST( test1_AlignCheck, stressTestAlign_group)
{
	// Initialize the memory system
	Memory::Initialize();

   // create heap
   Memory::Code memResult;

   Heap *heap_A;
   memResult = Memory::HeapCreate(heap_A, 50 * 1024, "HEAP_A");
   CHECK( memResult == Memory::OK );

   char *p0 = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__ ) char[200];
   char *p1 = new(heap_A, Memory::Align::Byte_16, __FILE__, __LINE__ ) char[200];
   char *p2 = new(heap_A, Memory::Align::Byte_16, __FILE__, __LINE__ ) char[200];
   char *p3 = new(heap_A, Memory::Align::Byte_16, __FILE__, __LINE__ ) char[200];
   char *p4 = new(heap_A, Memory::Align::Byte_64, __FILE__, __LINE__ ) char[200];
   char *p5 = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__ ) char[200];
   char *p6 = new(heap_A, Memory::Align::Byte_64, __FILE__, __LINE__ ) char[200];
   char *p7 = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__ ) char[200];
   char *p8 = new(heap_A, Memory::Align::Byte_256, __FILE__, __LINE__ ) char[200];
   char *p9 = new(heap_A, Memory::Align::Byte_256, __FILE__, __LINE__ ) char[200];

   CHECK( TEST_4_BYTE(p0) );
   CHECK( TEST_16_BYTE(p1) );
   CHECK( TEST_16_BYTE(p2) );
   CHECK( TEST_16_BYTE(p3) );
   CHECK( TEST_64_BYTE(p4) );
   CHECK( TEST_4_BYTE(p5) );
   CHECK( TEST_64_BYTE(p6) );
   CHECK( TEST_4_BYTE(p7) );
   CHECK( TEST_256_BYTE(p8) );
   CHECK( TEST_256_BYTE(p9) );

   Memory::Info info;
   Memory::GetInfo( info );

   // CHECK the global memory
   CHECK( info.CurrAllocIndex == 10 );
   CHECK( info.CurrBytesUsed  == 10*(200) );
   CHECK( info.CurrHeapCount == 1 );
   CHECK( info.CurrNumAlloc == 10 );
   CHECK( info.PeakBytesUsed == 10*(200) );
   CHECK( info.PeakHeapCount == 1 );
   CHECK( info.PeakNumAlloc == 10 );

   // CHECK the memory of each HEAP
   Heap::Info HeapInfo;

   heap_A->GetInfo( HeapInfo );
   CHECK( HeapInfo.CurrBytesUsed == 10*(200) );
   CHECK( HeapInfo.CurrNumAlloc == 10 );
   CHECK( strcmp(HeapInfo.Name, "HEAP_A") == 0 );
   CHECK( HeapInfo.PeakBytesUsed == 200*(10) );
   CHECK( HeapInfo.PeakNumAlloc == 10 );
   CHECK( HeapInfo.TotalSize == 50*1024 );

   	Memory::Destroy();
}


TEST( test1_alignCheck2, stressTestAlign_group)
{
	// Initialize the memory system
	Memory::Initialize();

   // create heap
   Memory::Code memResult;
   Heap *heap_A;
   memResult = Memory::HeapCreate(heap_A, 50 * 1024, "HEAP_A");
   CHECK( memResult == Memory::OK );

   char *p0 = new(heap_A, Memory::Align::Byte_16, __FILE__, __LINE__ ) char[200];
   char *p1 = new(heap_A, Memory::Align::Byte_16, __FILE__, __LINE__ ) char[200];
   char *p2 = new(heap_A, Memory::Align::Byte_16, __FILE__, __LINE__ ) char[200];
   char *p3 = new(heap_A, Memory::Align::Byte_16, __FILE__, __LINE__ ) char[200];
   char *p4 = new(heap_A, Memory::Align::Byte_64, __FILE__, __LINE__ ) char[200];
   char *p5 = new(heap_A, Memory::Align::Byte_64, __FILE__, __LINE__ ) char[200];
   char *p6 = new(heap_A, Memory::Align::Byte_64, __FILE__, __LINE__ ) char[200];
   char *p7 = new(heap_A, Memory::Align::Byte_64, __FILE__, __LINE__ ) char[200];
   char *p8 = new(heap_A, Memory::Align::Byte_256, __FILE__, __LINE__ ) char[200];
   char *p9 = new(heap_A, Memory::Align::Byte_256, __FILE__, __LINE__ ) char[200];
   char *p10 = new(heap_A, Memory::Align::Byte_8, __FILE__, __LINE__ ) char[200];
   char *p11 = new(heap_A, Memory::Align::Byte_32, __FILE__, __LINE__ ) char[200];
   char *p12 = new(heap_A, Memory::Align::Byte_128, __FILE__, __LINE__ ) char[200];
   char *p13 = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__ ) char[200];

   CHECK( TEST_16_BYTE(p0) );
   CHECK( TEST_16_BYTE(p1) );
   CHECK( TEST_16_BYTE(p2) );
   CHECK( TEST_16_BYTE(p3) );
   CHECK( TEST_64_BYTE(p4) );
   CHECK( TEST_64_BYTE(p5) );
   CHECK( TEST_64_BYTE(p6) );
   CHECK( TEST_64_BYTE(p7) );
   CHECK( TEST_256_BYTE(p8) );
   CHECK( TEST_256_BYTE(p9) );
   CHECK( TEST_8_BYTE(p10) );
   CHECK( TEST_32_BYTE(p11) );
   CHECK( TEST_128_BYTE(p12) );
   CHECK( TEST_4_BYTE(p13) );

   Memory::Info info;
   Memory::GetInfo( info );

   // CHECK the global memory
   CHECK( info.CurrAllocIndex == 14 );
   CHECK( info.CurrBytesUsed  == 14*(200) );
   CHECK( info.CurrHeapCount == 1 );
   CHECK( info.CurrNumAlloc == 14 );
   CHECK( info.PeakBytesUsed == 14*(200) );
   CHECK( info.PeakHeapCount == 1 );
   CHECK( info.PeakNumAlloc == 14 );

   // CHECK the memory of each HEAP
   Heap::Info HeapInfo;

   heap_A->GetInfo( HeapInfo );
   CHECK( HeapInfo.CurrBytesUsed == 14*(200) );
   CHECK( HeapInfo.CurrNumAlloc == 14 );
   CHECK( strcmp(HeapInfo.Name, "HEAP_A") == 0 );
   CHECK( HeapInfo.PeakBytesUsed == 200*(14) );
   CHECK( HeapInfo.PeakNumAlloc == 14 );
   CHECK( HeapInfo.TotalSize == 50*1024 );

   	Memory::Destroy();
}

// ---  End of File ---------------
