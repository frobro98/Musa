#pragma once

#include "EngineCore/MemoryUtilities.h"
#include "Containers/DynamicArray.hpp"
#include "Containers/StaticArray.hpp"
#include "ECS/ComponentType.hpp"
#include "ECS/ComponentOffsetList.hpp"

namespace Musa
{
template <size_t N>
static uint64 TypeListHash(const StaticArray<ComponentType*, N> types)
{
	uint64 typelistHash = 0;
	for (const auto& type : types)
	{
		// TODO - This might not be the correct solution. Main problem is that there currently would be a comparison
		// and then a search over the archetype to verify correctness. If this isn't correctable, then this might be ok...?
		typelistHash |= types->archetypeBit;
	}
	return typelistHash;
}

constexpr size_t ArchetypeBlockSize = KilobytesAsBytes(16);

struct ArchetypeDataFooter
{
	struct Archetype* owner = nullptr;
	uint32 numEntities;
};

constexpr uint32 UsableChunkSize = ArchetypeBlockSize - sizeof(ArchetypeDataFooter);

struct /*alignas(32)*/ ArchetypeData
{
	uint8 data[UsableChunkSize];
	ArchetypeDataFooter header;
};
static_assert(sizeof(ArchetypeData) == ArchetypeBlockSize);


// Contains blocks of memory which contain the actual component data
// Similar to how Unity ECS archetypes are implemented
struct Archetype
{
	// Needs to store blocks of memory that contain component data contiguously

	// Must have some sort of identifier which also shows what components are stored within it
	DynamicArray<ArchetypeData*> chunkList;
};

static inline ComponentOffsetList ConstructOffsetList(const ComponentType** compTypes, size_t count)
{
	uint32 componentSetSize = 0;
	for (size_t i = 0; i < count; ++i)
	{
		componentSetSize += compTypes[i]->size;
	}


	constexpr uint32 usableSpace = UsableChunkSize;
	// slack just pads chunk for safety.
	// TODO - Investigate if this is actually necessary...
	constexpr uint32 slack = 2;
	const uint32 entityCapacity = usableSpace / componentSetSize - slack;

	DynamicArray<ComponentOffset> list;
	for (size_t i = 0; i < count; ++i)
	{

	}


	return ComponentOffsetList{
		std::move(list),
		entityCapacity
	};
}
}