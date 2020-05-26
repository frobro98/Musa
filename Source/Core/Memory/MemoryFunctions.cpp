#include "MemoryFunctions.hpp"
#include "Platform/PlatformMemory.hpp"

#include <malloc.h>

namespace Memory
{
void InitializeMemory()
{
	PlatformMemoryInfo memInfo = GetPlatformMemoryInfo();

	// Use memory info to generate however many allocation meta data buckets there are, forever
}
void* Memory::Malloc(size_t size, size_t alignment)
{
	// Check size to see if it's going to end up in the fixed block allocation pools
	return _aligned_malloc(size, alignment);
}

void* Memory::Realloc(void* ptr, size_t size, size_t alignment)
{
	// Checks the actual size of the memory within the ptrs current allocation
	// If it can fit the new size within the current block, it just returns that ptr
	// else it uses Malloc and Free to get a new allocation
	return _aligned_realloc(ptr, size, alignment);
}

void Memory::Free(void* p)
{
	// Main issue here is verifying that the pointer that comes back is in memory that this system has allocated

	// The way I think would work is having a fixed-size collection of meta data where each 

	_aligned_free(p);
}

}

