// Copyright 2020, Nathan Blane

#pragma once

#include "Platform.h"
#include "Utilities/Hash.hpp"
#include "String/CStringUtilities.hpp"
#include "GUID/Guid.hpp"

enum class KeyInput;

constexpr forceinline uint32 GetHash(uint8 b)
{
	return static_cast<uint32>(b);
}

constexpr forceinline uint32 GetHash(int8 b)
{
	return static_cast<uint32>(b);
}

constexpr forceinline uint32 GetHash(uint16 s)
{
	return static_cast<uint32>(s);
}

constexpr forceinline uint32 GetHash(int16 s)
{
	return static_cast<uint32>(s);
}

constexpr forceinline uint32 GetHash(uint32 i)
{
	return i;
}

constexpr forceinline uint32 GetHash(int32 i)
{
	return static_cast<uint32>(i);
}

constexpr forceinline uint32 GetHash(uint64 i)
{
	return fnv32(&i, sizeof(uint64));
}

constexpr forceinline uint32 GetHash(int64 i)
{
	return fnv32(&i, sizeof(int64));
}

forceinline uint32 GetHash(float f)
{
	return *reinterpret_cast<uint32*>(&f);
}

inline uint32 GetHash(double d)
{
	return fnv32(&d, sizeof(double));
}

constexpr forceinline uint32 GetHash(const void* p)
{
	return fnv32(p, sizeof(p));
}

constexpr forceinline uint32 GetHash(void* p)
{
	return fnv32(p, sizeof(p));
}

constexpr forceinline uint32 GetHash(const tchar* cStr)
{
	return fnv32(cStr);
}

constexpr forceinline uint32 GetHash(KeyInput key)
{
	return static_cast<uint32>(key);
}

forceinline uint32 GetHash(const Guid& guid)
{
	return guid.GetHash();
}