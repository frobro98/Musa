#pragma once

#include "Platform.h"
#include "FNV-1a.h"
#include "String/CStringUtilities.hpp"
#include "GUID/Guid.hpp"

enum class KeyInput;

inline uint32 GetHash(uint8 b)
{
	return static_cast<uint32>(b);
}

inline uint32 GetHash(int8 b)
{
	return static_cast<uint32>(b);
}

inline uint32 GetHash(uint16 s)
{
	return static_cast<uint32>(s);
}

inline uint32 GetHash(int16 s)
{
	return static_cast<uint32>(s);
}

inline uint32 GetHash(uint32 i)
{
	return i;
}

inline uint32 GetHash(int32 i)
{
	return static_cast<uint32>(i);
}

inline uint32 GetHash(uint64 i)
{
	return fnv(&i, sizeof(uint64));
}

inline uint32 GetHash(int64 i)
{
	return fnv(&i, sizeof(int64));
}

inline uint32 GetHash(float f)
{
	return *reinterpret_cast<uint32*>(&f);
}

inline uint32 GetHash(double d)
{
	return fnv(&d, sizeof(double));
}

inline uint32 GetHash(const void* p)
{
	return fnv(p, sizeof(p));
}

inline uint32 GetHash(void* p)
{
	return fnv(p, sizeof(p));
}

inline uint32 GetHash(const tchar* cStr)
{
	return fnv(cStr, Strlen(cStr));
}

inline uint32 GetHash(KeyInput key)
{
	return static_cast<uint32>(key);
}

inline uint32 GetHash(const Guid& guid)
{
	return guid.GetHash();
}