#pragma once

#include "Types/Intrinsics.hpp"

namespace Musa
{
template <typename CompType>
struct ChunkArray
{
	ChunkArray() = default;
	ChunkArray(CompType& compBegin, uint32 size);

	CompType& operator[](size_t index);
	const CompType& operator[](size_t index) const;

	uint32 Size() const;
	bool IsValid() const;

	friend CompType* begin(ChunkArray& arr) { return arr.data; }
	friend const CompType* begin(const ChunkArray& arr) { return arr.data; }
	friend CompType* end(ChunkArray& arr) { return arr.data + arr.numEntities; }
	friend const CompType* end(const ChunkArray& arr) { return arr.data + arr.numEntities; }

	CompType* data = nullptr;
	uint32 numEntities = 0;
};

template <typename CompType>
ChunkArray<CompType>::ChunkArray(CompType& compBegin, uint32 size)
{
	REF_CHECK(compBegin);
	data = &compBegin;
	numEntities = size;
}

template <typename CompType>
inline CompType& ChunkArray<CompType>::operator[](size_t index)
{
	Assert(index < Size());
	return data[index];
}

template <typename CompType>
inline const CompType& ChunkArray<CompType>::operator[](size_t index) const
{
	Assert(index < Size());
	return data[index];
}

template <typename CompType>
inline uint32 ChunkArray<CompType>::Size() const
{
	return numEntities;
}

template <typename CompType>
inline bool ChunkArray<CompType>::IsValid() const
{
	return data != nullptr;
}
}

