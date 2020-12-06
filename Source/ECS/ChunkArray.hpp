// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"

namespace Musa
{
template <typename CompType>
struct ChunkArray
{
	ChunkArray();
	ChunkArray(CompType& compBegin, u32 count);

	CompType& operator[](size_t index);
	const CompType& operator[](size_t index) const;

	bool IsValid() const;

	friend CompType* begin(ChunkArray& arr) { return arr.data; }
	friend const CompType* begin(const ChunkArray& arr) { return arr.data; }
	friend CompType* end(ChunkArray& arr) { return arr.data + arr.size; }
	friend const CompType* end(const ChunkArray& arr) { return arr.data + arr.size; }

	CompType* const data;
	const u32 size;
};


template <typename CompType>
ChunkArray<CompType>::ChunkArray()
	: data(nullptr),
	size(0)
{
}

template <typename CompType>
ChunkArray<CompType>::ChunkArray(CompType& compBegin, u32 count)
	: data(&compBegin),
	size(count)
{
	REF_CHECK(compBegin);
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

