#pragma once

#include "PlatformDefinitions.h"
#include "String/CStringUtilities.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////
// FNV hash only supports 32 bit numbers currently
//
// If 64 bit numbers are a necessity, consider the
// implementation AudioKinetic did at:
// https://www.audiokinetic.com/library/2015.1_5418/?source=SDK&id=_ak_f_n_v_hash_8h_source.html
////////////////////////////////////////////////////////////////////////////////////////////////

constexpr uint32 fnv(const void* hashData, uint32 dataSize)
{
	uint8* byteData = (uint8 *)hashData;

	// These numbers were gotten off of this website
	// http://www.isthe.com/chongo/tech/comp/fnv/#FNV-param
	constexpr uint32 offsetBasis = 2166136261;
	constexpr uint32 hashPrime = 16777619;

	uint32 hash = offsetBasis;

	for (uint32 i = 0; i < dataSize; ++i)
	{
		hash ^= *byteData++;
		hash *= hashPrime;
	}

	return hash;
}

template <class T, typename = std::enable_if_t<!std::is_pointer_v<T>>>
constexpr inline uint32 fnvHash(const T& objToHash)
{
	return fnv(&objToHash, sizeof(T));
}

constexpr inline uint32 fnvHash(const tchar* objToHash)
{
	return fnv(objToHash, Strlen(objToHash));
}
