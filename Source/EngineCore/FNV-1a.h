#pragma once

#include "PlatformDefinitions.h"
#include "String/CStringUtilities.hpp"

constexpr inline uint32 fnv32(const void* hashData, uint32 dataSize)
{
	uint8* byteData = (uint8 *)hashData;

	// These numbers were gotten off of this website
	// http://www.isthe.com/chongo/tech/comp/fnv/#FNV-param
	constexpr uint32 offsetBasis = 0x811c9dc5;
	constexpr uint32 hashPrime = 0x1000193;

	uint32 hash = offsetBasis;

	for (uint32 i = 0; i < dataSize; ++i)
	{
		hash ^= *byteData++;
		hash *= hashPrime;
	}

	return hash;
}

constexpr inline uint32 fnv32(const tchar* strData)
{
	// These numbers were gotten off of this website
	// http://www.isthe.com/chongo/tech/comp/fnv/#FNV-param
	constexpr uint32 offsetBasis = 0x811c9dc5;
	constexpr uint32 hashPrime = 0x1000193;

	uint32 hash = offsetBasis;

	uint32 i = 0;
	while (strData[i])
	{
		hash ^= strData[i++];
		hash *= hashPrime;
	}

	return hash;
}

constexpr inline uint64 fnv64(const void* hashData, uint64 dataSize)
{
	uint8* byteData = (uint8 *)hashData;

	// These numbers were gotten off of this website
	// http://www.isthe.com/chongo/tech/comp/fnv/#FNV-param
	constexpr uint64 offsetBasis = 0xcbf29ce484222325;
	constexpr uint64 hashPrime = 0x100000001b3;

	uint64 hash = offsetBasis;

	for (uint64 i = 0; i < dataSize; ++i)
	{
		hash ^= *byteData++;
		hash *= hashPrime;
	}

	return hash;
}

constexpr inline uint64 fnv64(const tchar* strData)
{
	// These numbers were gotten off of this website
	// http://www.isthe.com/chongo/tech/comp/fnv/#FNV-param
	constexpr uint64 offsetBasis = 0xcbf29ce484222325;
	constexpr uint64 hashPrime = 0x100000001b3;

	uint64 hash = offsetBasis;

	uint64 i = 0;
	while(strData[i])
	{
		hash ^= strData[i++];
		hash *= hashPrime;
	}

	return hash;
}

template <class T, typename = std::enable_if_t<!std::is_pointer_v<T>>>
constexpr inline uint64 fnvHash(const T& objToHash)
{
	return fnv64(&objToHash, sizeof(T));
}

