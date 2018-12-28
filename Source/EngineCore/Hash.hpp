#pragma once

#include "FNV-1a.h"

struct FNV1aHash
{
	uint32 hash;

	template <class T, typename = std::enable_if_t<!std::is_pointer_v<T>>>
	FNV1aHash(const T& objToHash)
	{
		hash = fnv(&objToHash, sizeof(T));
	}


};