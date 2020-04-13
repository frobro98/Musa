#pragma once

#include "ECS/Archetype.hpp"

namespace Musa
{
template <typename CompType>
struct ComponentArray
{
	ComponentArray() = default;
	ComponentArray(CompType& compBegin, uint32 size);

	CompType& operator[](size_t index);
	const CompType& operator[](size_t index) const;

	uint32 Size() const;
	bool IsValid() const;

	friend CompType* begin(ComponentArray& arr) { return arr.data; }
	friend const CompType* begin(const ComponentArray& arr) { return arr.data; }
	friend CompType* end(ComponentArray& arr) { return arr.data + arr.owningChunk->footer.numEntities; }
	friend const CompType* end(const ComponentArray& arr) { return arr.data + arr.owningChunk->footer.numEntities; }

	CompType* data = nullptr;
	uint32 numEntities = 0;
};

template <typename CompType>
ComponentArray<CompType>::ComponentArray(CompType& compBegin, uint32 size)
{
	REF_CHECK(compBegin);
	data = &compBegin;
	numEntities = size;
}

template <typename CompType>
inline CompType& ComponentArray<CompType>::operator[](size_t index)
{
	Assert(index < Size());
	return data[index];
}

template <typename CompType>
inline const CompType& ComponentArray<CompType>::operator[](size_t index) const
{
	Assert(index < Size());
	return data[index];
}

template <typename CompType>
inline uint32 ComponentArray<CompType>::Size() const
{
	return owningChunk->footer.numEntities;
}

template <typename CompType>
inline bool ComponentArray<CompType>::IsValid() const
{
	return data != nullptr;
}
}

