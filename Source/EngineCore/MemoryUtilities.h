#pragma once

#include "Assertion.h"

inline constexpr size_t KilobytesAsBytes(size_t kilobytes)
{
	return kilobytes * 1024;
}

inline constexpr size_t MegabytesAsBytes(size_t megabytes)
{
	return KilobytesAsBytes(megabytes) * 1024;
}

inline constexpr size_t GigabytesAsBytes(size_t gigabytes)
{
	return MegabytesAsBytes(gigabytes) * 1024;
}

inline constexpr size_t Kilobytes(size_t bytes)
{
	return bytes / 1024;
}

inline constexpr size_t Megabytes(size_t bytes)
{
	return bytes / Kilobytes(bytes);
}

inline constexpr size_t Gigabytes(size_t bytes)
{
	return bytes / Megabytes(bytes);
}

inline void Memcpy(void* dest, size_t destSize,  const void* src, size_t srcSize)
{
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

constexpr bool IsPowerOf2(uint64 num)
{
	return (!(num == 0)) & ((num & (num - 1)) == 0);
}

template <typename AlignType>
constexpr AlignType Align(const AlignType ptr, uint64 alignment)
{
	Assert(IsPowerOf2(alignment));
	return (AlignType)(((uint64)ptr + (alignment - 1)) & ~(alignment - 1));
}



inline size_t* GetSecretPointer(const void* data)
{
	return (size_t*)(*(size_t*)((size_t)data - sizeof(size_t)));
}