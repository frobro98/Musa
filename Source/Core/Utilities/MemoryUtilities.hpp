// Copyright 2020, Nathan Blane

#pragma once

#include "CoreFlags.hpp"
#include "Debugging/Assertion.hpp"

forceinline constexpr size_t KilobytesAsBytes(size_t kilobytes)
{
	return kilobytes * 1024;
}

forceinline constexpr size_t MegabytesAsBytes(size_t megabytes)
{
	return KilobytesAsBytes(megabytes) * 1024;
}

forceinline constexpr size_t GigabytesAsBytes(size_t gigabytes)
{
	return MegabytesAsBytes(gigabytes) * 1024;
}

forceinline constexpr size_t Kilobytes(size_t bytes)
{
	return bytes / 1024;
}

forceinline constexpr size_t Megabytes(size_t bytes)
{
	return bytes / Kilobytes(bytes);
}

forceinline constexpr size_t Gigabytes(size_t bytes)
{
	return bytes / Megabytes(bytes);
}

forceinline void Memcpy(void* dst, const void* src, size_t size)
{
	memcpy(dst, src, size);
}

forceinline void Memcpy(void* dest, size_t destSize,  const void* src, size_t srcSize)
{
	memcpy_s(dest, destSize, src, srcSize);
}

forceinline void Memmove(void* dest, size_t destSize, const void* src, size_t srcSize)
{
	memmove_s(dest, destSize, src, srcSize);
}

forceinline void Memset(void* mem, int8 value, size_t memSize)
{
	memset(mem, value, memSize);
}

forceinline void ZeroMem(void* mem, size_t memSize)
{
	memset(mem, 0, memSize);
}

forceinline int32 Memcmp(const void* src1, const void* src2, size_t count)
{
	return memcmp(src1, src2, count);
}

forceinline const void* Memchr(const void* ptr, tchar ch, size_t count)
{
	return memchr(ptr, ch, count);
}

forceinline constexpr bool IsPowerOf2(uint64 num)
{
	return (!(num == 0)) & ((num & (num - 1)) == 0);
}

template <typename AlignType>
forceinline constexpr AlignType Align(const AlignType ptr, size_t alignment)
{
	Assert(IsPowerOf2(alignment));
	return (AlignType)(((uint64)ptr + (alignment - 1)) & ~(alignment - 1));
}

forceinline constexpr bool IsAligned(uintptr_t ptrAddr, uintptr_t alignment)
{
	return !(ptrAddr % alignment);
}

forceinline bool IsAligned(const void* ptrAddr, uintptr_t alignment)
{
	uintptr_t ptr = reinterpret_cast<uintptr_t>(ptrAddr);
	return !(ptr % alignment);
}

forceinline size_t* GetSecretPointer(const void* data)
{
	return (size_t*)(*(size_t*)((size_t)data - sizeof(size_t)));
}