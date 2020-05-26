#pragma once

#include "BasicTypes/Intrinsics.hpp"


namespace Memory
{
// Defaulted to 16 because of vector intrinsics
constexpr size_t DefaultAlignment = 16;

void InitializeMemory();

void* Malloc(size_t size, size_t alignment = DefaultAlignment);

void* Realloc(void* ptr, size_t size, size_t alignment = DefaultAlignment);

void Free(void* p);
}


