// Copyright 2020, Nathan Blane

#pragma once

#include <type_traits>

#include "BasicTypes/Intrinsics.hpp"

template <typename T>
forceinline void HashCombine(uint32& hash, const T& obj)
{
	uint32 objHash = GetHash(obj);
	HashCombine(hash, objHash);
}

forceinline void HashCombine(uint32& hash, uint32 val)
{
	hash ^= val + 0x9e3779b9 + (hash << 6) + (hash >> 2);
}

constexpr uint32 fnv32(const void* hashData, uint32 dataSize)
{
	uint8* byteData = (uint8 *)hashData;

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

constexpr uint32 fnv32(const tchar* strData)
{
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

constexpr uint64 fnv64(const void* hashData, uint64 dataSize)
{
	uint8* byteData = (uint8 *)hashData;

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

constexpr uint64 fnv64(const tchar* strData)
{
	constexpr uint64 offsetBasis = 0xcbf29ce484222325;
	constexpr uint64 hashPrime = 0x100000001b3;

	uint64 hash = offsetBasis;

	uint64 i = 0;
	while (strData[i])
	{
		hash ^= strData[i++];
		hash *= static_cast<uint64>(hashPrime);
	}

	return hash;
}

template <class T, typename = std::enable_if_t<!std::is_pointer_v<T>>>
constexpr uint64 fnvHash(const T& objToHash)
{
	return fnv64(&objToHash, sizeof(T));
}


constexpr uint32 XXHash32Prime1 = 0x9e3779b1;
constexpr uint32 XXHash32Prime2 = 0x85ebca77;
constexpr uint32 XXHash32Prime3 = 0xc2b2ae3d;
constexpr uint32 XXHash32Prime4 = 0x27d4eb2f;
constexpr uint32 XXHash32Prime5 = 0x165667b1;

constexpr uint64 XXHash64Prime1 = 0x9e3779b185ebca87;
constexpr uint64 XXHash64Prime2 = 0xc2b2ae3d27d4eb4f;
constexpr uint64 XXHash64Prime3 = 0x165667b19e3779f9;
constexpr uint64 XXHash64Prime4 = 0x85ebca77c2b2ae63;
constexpr uint64 XXHash64Prime5 = 0x27d4eb2f165667C5;


// TODO - Move this rotation to a place that can be reused. Might be useful to have...
constexpr forceinline uint32 xxhashRotl(uint32 val, uint32 bits)
{
	return (val << bits) | (val >> (32 - bits));
}

// TODO - Move this rotation to a place that can be reused. Might be useful to have...
constexpr forceinline uint64 xxhashRotl(uint64 val, uint64 bits)
{
	return (val << bits) | (val >> (64 - bits));
}

constexpr forceinline uint32 xxhashRound32(uint32 input, uint32 accum)
{
	uint32 res = accum + input * XXHash32Prime2;
	res = xxhashRotl(res, 13);
	res *= XXHash32Prime1;
	return res;
}

constexpr forceinline uint64 xxhashRound64(uint64 input, uint64 accum)
{
	uint64 res = accum + input * XXHash64Prime2;
	res = xxhashRotl(res, 31);
	res *= XXHash64Prime1;
	return res;
}

constexpr forceinline uint64 xxhashCombineRound(uint64 input, uint64 accum)
{
	uint64 newVal = xxhashRound64(input, 0);
	uint64 res = accum ^ newVal;
	res = res * XXHash64Prime1 + XXHash64Prime4;
	return res;
}

uint32 xxhash32(const void* buffer, uint32 byteCount, uint32 seed = 0);


// Doesn't work currently. Not sure why it doesn't, but the need for a 64 bit hash currently isn't there
// uint64 xxhash64(const void* buffer, uint64 byteCount, uint64 seed)
// {
// 	uint64 hashRes = 0;
// 	const uint8* end = buffer + byteCount;
// 	const uint64* int64Ptr = reinterpret_cast<const uint64*>(buffer);
// 	if (byteCount >= 32)
// 	{
// 		const uint8* limit = end - 32;
// 		uint64 v0 = XXHash64Prime1 + XXHash64Prime2 + seed;
// 		uint64 v1 = XXHash64Prime2 + seed;
// 		uint64 v2 = 0 + seed;
// 		uint64 v3 = seed - XXHash64Prime1;
// 
// 		//uint64 loopCount = (byteCount - 32) / 8;
// 		//uint64 ops = 0;
// 		do
// 		{
// 			v0 = xxhashRound64(*int64Ptr++, v0); // op 1
// 			v1 = xxhashRound64(*int64Ptr++, v1); // op 2
// 			v2 = xxhashRound64(*int64Ptr++, v2); // op 3
// 			v3 = xxhashRound64(*int64Ptr++, v3); // op 4
// 
// 			//ops += 4;
// 		} while ((const uint8*)int64Ptr < limit);
// 
// 		hashRes = xxhashRotl(v0, 1) + xxhashRotl(v1, 7) + xxhashRotl(v2, 12) + xxhashRotl(v3, 18);
// 		hashRes = xxhashCombineRound(v0, hashRes);
// 		hashRes = xxhashCombineRound(v1, hashRes);
// 		hashRes = xxhashCombineRound(v2, hashRes);
// 		hashRes = xxhashCombineRound(v3, hashRes);
// 	}
// 	else
// 	{
// 		hashRes = seed + XXHash64Prime5;
// 	}
// 
// 	hashRes += byteCount;
// 
// 	uint64 fiveBitCount = byteCount & 31;
// 	while (fiveBitCount >= 8)
// 	{
// 		uint64 const k1 = xxhashRound64(*int64Ptr, 0);
// 		++int64Ptr;
// 		hashRes ^= k1;
// 		hashRes = xxhashRotl(hashRes, 27) * XXHash64Prime1 + XXHash64Prime4;
// 		fiveBitCount -= 8;
// 	}
// 
// 	const uint32* int32Ptr = reinterpret_cast<const uint32*>(int64Ptr);
// 	if (fiveBitCount >= 4)
// 	{
// 		hashRes ^= (uint64)(*int32Ptr) * XXHash64Prime1;
// 		++int32Ptr;
// 		hashRes = xxhashRotl(hashRes, 23) * XXHash64Prime2 + XXHash64Prime3;
// 		fiveBitCount -= 4;
// 	}
// 
// 	const uint8* bytePtr = reinterpret_cast<const uint8*>(int32Ptr);
// 	while (fiveBitCount > 0)
// 	{
// 		hashRes ^= *bytePtr++ * XXHash64Prime5;
// 		hashRes = xxhashRotl(hashRes, 11) * XXHash64Prime1;
// 		--fiveBitCount;
// 	}
// 
// 	hashRes ^= hashRes >> 33;
// 	hashRes *= XXHash64Prime2;
// 	hashRes ^= hashRes >> 29;
// 	hashRes *= XXHash64Prime3;
// 	hashRes ^= hashRes >> 32;
// 
// 	return hashRes;
// }

