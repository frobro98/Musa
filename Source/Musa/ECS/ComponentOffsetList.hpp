#pragma once

#include "Types/Intrinsics.hpp"
#include "Containers/DynamicArray.hpp"

struct ComponentType;

struct ComponentOffset
{
	const ComponentType* type = nullptr;
	uint32 chunkOffset = 0;
};

struct ComponentOffsetList
{
	DynamicArray<ComponentOffset> offsets;
	const uint32 capacity;
};

