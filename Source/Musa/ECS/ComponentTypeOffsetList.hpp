#pragma once

#include "Types/Intrinsics.hpp"
#include "Containers/DynamicArray.hpp"

namespace Musa
{
struct ComponentType;

struct ComponentTypeOffset
{
	const ComponentType* type = nullptr;
	const uint64 typenameHash;
	const uint64 archetypeBit;
	const size_t chunkOffset;
};

struct ComponentTypeOffsetList
{
	const DynamicArray<ComponentTypeOffset> offsets;
	const uint32 capacity;
};

// TODO - Investigate compile time construction...
ComponentTypeOffsetList ConstructOffsetList(const ComponentType** compTypes, size_t count);
}

