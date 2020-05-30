// Copyright 2020, Nathan Blane

#pragma once

// TODO - Remove this by implementing your own versions of memcpy, memset, etc.
#include <cstring>

#include "BasicTypes/Intrinsics.hpp"
#include "CoreAPI.hpp"

namespace Memory
{
// Defaulted to 16 because of vector intrinsics
constexpr size_t DefaultAlignment = 16;

CORE_API void* Malloc(size_t size, size_t alignment = DefaultAlignment);

CORE_API void* Realloc(void* ptr, size_t size, size_t alignment = DefaultAlignment);

CORE_API void Free(void* p);

forceinline void Memcpy(void* dst, const void* src, size_t size)
{
	memcpy(dst, src, size);
}

forceinline void Memmove(void* dst, const void* src, size_t size)
{
	memmove(dst, src, size);
}

forceinline void Memset(void* mem, i8 value, size_t memSize)
{
	memset(mem, value, memSize);
}

forceinline void Memzero(void* mem, size_t memSize)
{
	Memset(mem, 0, memSize);
}

forceinline i32 Memcmp(const void* src1, const void* src2, size_t count)
{
	return memcmp(src1, src2, count);
}

forceinline const void* Memchr(const void* ptr, tchar ch, size_t count)
{
	return memchr(ptr, ch, count);
}
}


