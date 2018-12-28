#pragma once

#include "Platform.h"

inline void Memcpy(void* dest, size_t destSize,  const void* src, size_t srcSize)
{
	// TODO - memcpy_s probably isn't cross platform...figure out if it is
	memcpy_s(dest, destSize, src, srcSize);
}

inline void Memmove(void* dest, size_t destSize, const void* src, size_t srcSize)
{
	memmove_s(dest, destSize, src, srcSize);
}

inline void Memset(void* mem, int32 value, size_t memSize)
{
	memset(mem, value, memSize);
}

inline void ZeroMem(void* mem, size_t memSize)
{
	memset(mem, 0, memSize);
}

inline int32 Memcmp(const void* src1, const void* src2, size_t count)
{
	return memcmp(src1, src2, count);
}

inline const void* Memchr(const void* ptr, tchar ch, size_t count)
{
	return memchr(ptr, ch, count);
}

constexpr bool IsPowerOf2(uint32 num)
{
	return (!(num == 0)) & ((num & (num - 1)) == 0);
}

template <typename AlignType>
constexpr AlignType Align(const AlignType ptr, uint32 alignment)
{
	return (AlignType)(((uint32)ptr + (alignment - 1)) & ~(alignment - 1));
}


inline unsigned int* GetSecretPointer(const void* data)
{
	return (unsigned int*)(*(unsigned int*)((unsigned int)data - sizeof(size_t)));
}