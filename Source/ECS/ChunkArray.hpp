// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"

namespace Musa
{
template <typename CompType>
struct ChunkArray
{
	ChunkArray() = default;
	ChunkArray(CompType& compBegin, uint32 count);

	CompType& operator[](size_t index);
	const CompType& operator[](size_t index) const;

	bool IsValid() const;

	friend CompType* begin(ChunkArray& arr) { return arr.data; }
	friend const CompType* begin(const ChunkArray& arr) { return arr.data; }
	friend CompType* end(ChunkArray& arr) { return arr.data + arr.size; }
	friend const CompType* end(const ChunkArray& arr) { return arr.data + arr.size; }

	CompType* data = nullptr;
	uint32 size = 0;
};

template <typename CompType>
ChunkArray<CompType>::ChunkArray(CompType& compBegin, uint32 count)
{
	REF_CHECK(compBegin);
	data = &compBegin;
	size = count;
}

template <typename CompType>
inline CompType& ChunkArray<CompType>::operator[](size_t index)
{
	Assert(index < size);
	return data[index];
}

template <typename CompType>
inline const CompType& ChunkArray<CompType>::operator[](size_t index) const
{
	Assert(index < size);
	return data[index];
}

template <typename CompType>
inline bool ChunkArray<CompType>::IsValid() const
{
	return data != nullptr;
}
}

