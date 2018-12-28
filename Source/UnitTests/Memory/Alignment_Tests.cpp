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

#define TEST_4_BYTE(ptr)      ( ((unsigned int)ptr & 0x3) == 0 )
#define TEST_8_BYTE(ptr)      ( ((unsigned int)ptr & 0x7) == 0 )
#define TEST_16_BYTE(ptr)      ( ((unsigned int)ptr & 0xF) == 0 )
#define TEST_32_BYTE(ptr)      ( ((unsigned int)ptr & 0x1F) == 0 )
#define TEST_64_BYTE(ptr)      ( ((unsigned int)ptr & 0x3F) == 0 )
#define TEST_128_BYTE(ptr)      ( ((unsigned int)ptr & 0x7F) == 0 )
#define TEST_256_BYTE(ptr)      ( ((unsigned int)ptr & 0xFF) == 0 )

//---------------------------------------------------------------------------
// Test Files:
// Single step throught, set break points, but do not modify it
//---------------------------------------------------------------------------

TEST( align_4byte_test, align_group)
{
	// Initialize the Memory system
	Memory::Initialize();

   // create heap
   Heap *heap_A;
   Memory::Code memResult;

   memResult = Memory::HeapCreate(heap_A, 100 * 1024, "HEAP_A");
   CHECK( memResult == Memory::OK);

   Dog *p0 = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__ ) Dog();
   Dog *p1 = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__ ) Dog();
   Dog *p2 = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__ ) Dog();
   Dog *p3 = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__ ) Dog();
   Dog *p4 = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__ ) Dog();
   Dog *p5 = new(heap_A, Memory::Align::Byte_4, __FILE__, __LINE__ ) Dog();

   CHECK( p0 != 0 );
   CHECK( p1 != 0 );
   CHECK( p2 != 0 );
   CHECK( p3 != 0 );
   CHECK( p4 != 0 );
   CHECK( p5 != 0 );
     
   CHECK( TEST_4_BYTE(p0) );
   CHECK( TEST_4_BYTE(p1) );
   CHECK( TEST_4_BYTE(p2) );
   CHECK( TEST_4_BYTE(p3) );
   CHECK( TEST_4_BYTE(p4) );
   CHECK( TEST_4_BYTE(p5) );
 
   delete p0;
   delete p1;
   delete p2;
   delete p3;
   delete p4;
   delete p5;

   Memory::Destroy();
}

TEST( align_8byte_test, align_group)
{
	// Initialize the memory system
	Memory::Initialize();

   // create heap
   Heap *heap_A;
   Memory::Code  memResult;
	memResult = Memory::HeapCreate(heap_A, 100 * 1024, "HEAP_A");
   CHECK( memResult == Memory::OK);

   Dog *p0 = new(heap_A, Memory::Align::Byte_8, __FILE__, __LINE__ ) Dog();
   Dog *p1 = new(heap_A, Memory::Align::Byte_8, __FILE__, __LINE__ ) Dog();
   Dog *p2 = new(heap_A, Memory::Align::Byte_8, __FILE__, __LINE__ ) Dog();
   Dog *p3 = new(heap_A, Memory::Align::Byte_8, __FILE__, __LINE__ ) Dog();
   Dog *p4 = new(heap_A, Memory::Align::Byte_8, __FILE__, __LINE__ ) Dog();
   Dog *p5 = new(heap_A, Memory::Align::Byte_8, __FILE__, __LINE__ ) Dog();

   CHECK( p0 != 0 );
   CHECK( p1 != 0 );
   CHECK( p2 != 0 );
   CHECK( p3 != 0 );
   CHECK( p4 != 0 );
   CHECK( p5 != 0 );

   CHECK( TEST_8_BYTE(p0) );
   CHECK( TEST_8_BYTE(p1) );
   CHECK( TEST_8_BYTE(p2) );
   CHECK( TEST_8_BYTE(p3) );
   CHECK( TEST_8_BYTE(p4) );
   CHECK( TEST_8_BYTE(p5) );
 
   delete p0;
   delete p1;
   delete p2;
   delete p3;
   delete p4;
   delete p5;

   Memory::Destroy();
}

TEST( align_16byte_test, align_group)
{
	// Initialize the memory system
	Memory::Initialize();

   // create heap
	Memory::Code  memResult;
	Heap *heap_A;
	memResult = Memory::HeapCreate( heap_A, 100 * 1024, "HEAP_A");
	CHECK( memResult == Memory::OK );

   Dog *p0 = new(heap_A, Memory::Align::Byte_16, __FILE__, __LINE__ ) Dog();
   Dog *p1 = new(heap_A, Memory::Align::Byte_16, __FILE__, __LINE__ ) Dog();
   Dog *p2 = new(heap_A, Memory::Align::Byte_16, __FILE__, __LINE__ ) Dog();
   Dog *p3 = new(heap_A, Memory::Align::Byte_16, __FILE__, __LINE__ ) Dog();
   Dog *p4 = new(heap_A, Memory::Align::Byte_16, __FILE__, __LINE__ ) Dog();
   Dog *p5 = new(heap_A, Memory::Align::Byte_16, __FILE__, __LINE__ ) Dog();

   CHECK( p0 != 0 );
   CHECK( p1 != 0 );
   CHECK( p2 != 0 );
   CHECK( p3 != 0 );
   CHECK( p4 != 0 );
   CHECK( p5 != 0 );

   CHECK( TEST_16_BYTE(p0) );
   CHECK( TEST_16_BYTE(p1) );
   CHECK( TEST_16_BYTE(p2) );
   CHECK( TEST_16_BYTE(p3) );
   CHECK( TEST_16_BYTE(p4) );
   CHECK( TEST_16_BYTE(p5) );
 
   delete p0;
   delete p1;
   delete p2;
   delete p3;
   delete p4;
   delete p5;

   Memory::Destroy();
}

TEST( align_32byte_test, align_group)
{
	// Initialize the memory system
	Memory::Initialize();

   // create heap
   Memory::Code  memResult;
   Heap *heap_A;
   memResult = Memory::HeapCreate(heap_A, 100 * 1024, "HEAP_A");
   CHECK( memResult == Memory::OK );

   Dog *p0 = new(heap_A, Memory::Align::Byte_32, __FILE__, __LINE__ ) Dog();
   Dog *p1 = new(heap_A, Memory::Align::Byte_32, __FILE__, __LINE__ ) Dog();
   Dog *p2 = new(heap_A, Memory::Align::Byte_32, __FILE__, __LINE__ ) Dog();
   Dog *p3 = new(heap_A, Memory::Align::Byte_32, __FILE__, __LINE__ ) Dog();
   Dog *p4 = new(heap_A, Memory::Align::Byte_32, __FILE__, __LINE__ ) Dog();
   Dog *p5 = new(heap_A, Memory::Align::Byte_32, __FILE__, __LINE__ ) Dog();

   CHECK( p0 != 0 );
   CHECK( p1 != 0 );
   CHECK( p2 != 0 );
   CHECK( p3 != 0 );
   CHECK( p4 != 0 );
   CHECK( p5 != 0 );

   CHECK( TEST_32_BYTE(p0) );
   CHECK( TEST_32_BYTE(p1) );
   CHECK( TEST_32_BYTE(p2) );
   CHECK( TEST_32_BYTE(p3) );
   CHECK( TEST_32_BYTE(p4) );
   CHECK( TEST_32_BYTE(p5) );
 
   delete p0;
   delete p1;
   delete p2;
   delete p3;
   delete p4;
   delete p5;

   Memory::Destroy();

}

TEST( align_64byte_test, align_group)
{
	// Initialize the memory system
	Memory::Initialize();

   // create heap
   Memory::Code  memResult;
   Heap *heap_A;
   memResult = Memory::HeapCreate(heap_A, 100 * 1024, "HEAP_A");
   CHECK( memResult == Memory::OK );

   Dog *p0 = new(heap_A, Memory::Align::Byte_64, __FILE__, __LINE__ ) Dog();
   Dog *p1 = new(heap_A, Memory::Align::Byte_64, __FILE__, __LINE__ ) Dog();
   Dog *p2 = new(heap_A, Memory::Align::Byte_64, __FILE__, __LINE__ ) Dog();
   Dog *p3 = new(heap_A, Memory::Align::Byte_64, __FILE__, __LINE__ ) Dog();
   Dog *p4 = new(heap_A, Memory::Align::Byte_64, __FILE__, __LINE__ ) Dog();
   Dog *p5 = new(heap_A, Memory::Align::Byte_64, __FILE__, __LINE__ ) Dog();

   CHECK( p0 != 0 );
   CHECK( p1 != 0 );
   CHECK( p2 != 0 );
   CHECK( p3 != 0 );
   CHECK( p4 != 0 );
   CHECK( p5 != 0 );

   CHECK( TEST_64_BYTE(p0) );
   CHECK( TEST_64_BYTE(p1) );
   CHECK( TEST_64_BYTE(p2) );
   CHECK( TEST_64_BYTE(p3) );
   CHECK( TEST_64_BYTE(p4) );
   CHECK( TEST_64_BYTE(p5) );
 
   delete p0;
   delete p1;
   delete p2;
   delete p3;
   delete p4;
   delete p5;

   Memory::Destroy();
}

TEST( align_128byte_test, align_group)
{
	// Initialize the memory system
	Memory::Initialize();

   // create heap
   Memory::Code  memResult;
   Heap *heap_A;
   memResult = Memory::HeapCreate(heap_A, 100 * 1024, "HEAP_A");
   CHECK( memResult == Memory::OK );

   Dog *p0 = new(heap_A, Memory::Align::Byte_128, __FILE__, __LINE__ ) Dog();
   Dog *p1 = new(heap_A, Memory::Align::Byte_128, __FILE__, __LINE__ ) Dog();
   Dog *p2 = new(heap_A, Memory::Align::Byte_128, __FILE__, __LINE__ ) Dog();
   Dog *p3 = new(heap_A, Memory::Align::Byte_128, __FILE__, __LINE__ ) Dog();
   Dog *p4 = new(heap_A, Memory::Align::Byte_128, __FILE__, __LINE__ ) Dog();
   Dog *p5 = new(heap_A, Memory::Align::Byte_128, __FILE__, __LINE__ ) Dog();

   CHECK( p0 != 0 );
   CHECK( p1 != 0 );
   CHECK( p2 != 0 );
   CHECK( p3 != 0 );
   CHECK( p4 != 0 );
   CHECK( p5 != 0 );

   CHECK( TEST_128_BYTE(p0) );
   CHECK( TEST_128_BYTE(p1) );
   CHECK( TEST_128_BYTE(p2) );
   CHECK( TEST_128_BYTE(p3) );
   CHECK( TEST_128_BYTE(p4) );
   CHECK( TEST_128_BYTE(p5) );
 
   delete p0;
   delete p1;
   delete p2;
   delete p3;
   delete p4;
   delete p5;

   Memory::Destroy();
}

TEST( align_256byte_test, align_group)
{
	// Initialize the memory system
	Memory::Initialize();

   // create heap
   Memory::Code  memResult;
   Heap *heap_A;
   memResult = Memory::HeapCreate(heap_A, 100 * 1024, "HEAP_A");
   CHECK( memResult == Memory::OK );

   Dog *p0 = new(heap_A, Memory::Align::Byte_256, __FILE__, __LINE__ ) Dog();
   Dog *p1 = new(heap_A, Memory::Align::Byte_256, __FILE__, __LINE__ ) Dog();
   Dog *p2 = new(heap_A, Memory::Align::Byte_256, __FILE__, __LINE__ ) Dog();
   Dog *p3 = new(heap_A, Memory::Align::Byte_256, __FILE__, __LINE__ ) Dog();
   Dog *p4 = new(heap_A, Memory::Align::Byte_256, __FILE__, __LINE__ ) Dog();
   Dog *p5 = new(heap_A, Memory::Align::Byte_256, __FILE__, __LINE__ ) Dog();

   CHECK( p0 != 0 );
   CHECK( p1 != 0 );
   CHECK( p2 != 0 );
   CHECK( p3 != 0 );
   CHECK( p4 != 0 );
   CHECK( p5 != 0 );

   CHECK( TEST_256_BYTE(p0) );
   CHECK( TEST_256_BYTE(p1) );
   CHECK( TEST_256_BYTE(p2) );
   CHECK( TEST_256_BYTE(p3) );
   CHECK( TEST_256_BYTE(p4) );
   CHECK( TEST_256_BYTE(p5) );
 
   delete p0;
   delete p1;
   delete p2;
   delete p3;
   delete p4;
   delete p5;

   Memory::Destroy();
}

// ---  End of File ---------------
