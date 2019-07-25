//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#include "Memory.h"
#include "Heap.h"
#include "GlobalNew.h"
#include "Assertion.h"

// operator new
void * operator new( size_t inSize, Heap *pHeap, Memory::Align align, const char *inName, uint32 lineNum )
{
	return Memory::Malloc(inSize, pHeap, align, inName, lineNum);
}

void operator delete(void * /*p*/, Heap * /*inHeap*/, Memory::Align /*align*/, char * /*inName*/, int /*lineNum*/)
{
	Assert(false);
}

void * operator new( size_t inSize )
{
	void *p = malloc( inSize );
	return p;
}

// operator delete
void operator delete( void *p )
{
	if (p != nullptr)
	{
		Heap* heap = nullptr;
		if (Memory::GetHeapByAddr(heap, p) == Memory::OK)
		{
			Memory::Free(p, heap);
		}
		else
		{
			free(p);
		}
	}
}

void * operator new[]( size_t inSize ,Heap *pHeap, Memory::Align align, const char *inName, uint32 lineNum )
{
	return Memory::Malloc(inSize, pHeap, align, inName, lineNum);
}

void operator delete[](void * /*p*/, Heap * /*pHeap*/, Memory::Align /*align*/, char * /*inName*/, int /*lineNum*/)
{
	Assert(false);
}

void * operator new[]( size_t inSize )
{
	// TODO - Get at the first heap of the memory system and allocate it
	void *p = malloc( inSize );
	return p;
}

void operator delete[]( void *p )
{
	if (p != nullptr)
	{
		Heap* heap = nullptr;
		if (Memory::GetHeapByAddr(heap, p) == Memory::OK)
		{
			Memory::Free(p, heap);
		}
		else
		{
			free(p);
		}
	}
}

// ---  End of File ---------------
