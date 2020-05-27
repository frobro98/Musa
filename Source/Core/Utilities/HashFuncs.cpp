// Copyright 2020, Nathan Blane

#include "HashFuncs.hpp"

u32 xxhash32(const void * buffer, u32 byteCount, u32 seed)
{
	u32 hashRes = 0;
	const u32* intPtr = reinterpret_cast<const u32*>(buffer);

	hashRes = seed + XXHash32Prime5;
	if (byteCount >= 16)
	{
		u32 v0 = XXHash32Prime1 + XXHash32Prime2 + seed;
		u32 v1 = XXHash32Prime2 + seed;
		u32 v2 = 0 + seed;
		u32 v3 = seed - XXHash32Prime1;

		int count = byteCount >> 4;
		for (int i = 0; i < count; ++i)
		{
			v0 = xxhashRound32(*intPtr++, v0); // op 1
			v1 = xxhashRound32(*intPtr++, v1); // op 2
			v2 = xxhashRound32(*intPtr++, v2); // op 3
			v3 = xxhashRound32(*intPtr++, v3); // op 4
		}
		hashRes = xxhashRotl(v0, 1) + xxhashRotl(v1, 7) + xxhashRotl(v2, 12) + xxhashRotl(v3, 18);
	}

	hashRes += byteCount;

	// Process bytes less than 16
	u32 highBytes = (byteCount >> 2) & 3; // 0 1 1 0 -> 0 1
	for (u32 i = 0; i < highBytes; ++i)
	{
		hashRes += *intPtr++ * XXHash32Prime3;
		hashRes = xxhashRotl(hashRes, 17) * XXHash32Prime4;
	}

	const u8* bytePtr = reinterpret_cast<const u8*>(intPtr);
	u32 lowBytes = byteCount & 3; // 0 1 1 0 -> 1 0
	for (u32 i = 0; i < lowBytes; ++i)
	{
		hashRes += *bytePtr++ * XXHash32Prime5;
		hashRes = xxhashRotl(hashRes, 11) * XXHash32Prime1;
	}


	hashRes ^= hashRes >> 15;
	hashRes *= XXHash32Prime2;
	hashRes ^= hashRes >> 13;
	hashRes *= XXHash32Prime3;
	hashRes ^= hashRes >> 16;

	return hashRes;
}
