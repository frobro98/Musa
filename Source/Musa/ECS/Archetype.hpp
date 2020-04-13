#pragma once

#include "EngineCore/Algorithms.hpp"
#include "EngineCore/MemoryUtilities.h"
#include "Containers/DynamicArray.hpp"
#include "Containers/StaticArray.hpp"
#include "Utilities/TemplateUtils.hpp"
#include "ECS/Types.hpp"
#include "ECS/ComponentType.hpp"
#include "ECS/ComponentOffsetList.hpp"


namespace Musa
{
struct Component;

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

struct /*alignas(32)*/ ArchetypeChunk
{
	uint8 data[UsableChunkSize];
	ArchetypeDataFooter footer;
};
static_assert(sizeof(ArchetypeChunk) == ArchetypeBlockSize);


// Contains blocks of memory which contain the actual component data
// Similar to how Unity ECS archetypes are implemented
struct Archetype
{
	// Must have some sort of identifier which also shows what components are stored within it
	DynamicArray<ArchetypeChunk*> chunkList;
	ComponentOffsetList offsetList;
	ArchetypeHashID archetypeHashID;
	uint32 currentChunkIndex;
};

Archetype* CreateArchetypeFrom(const ComponentType** compTypes, size_t typeCount);

template<typename... Comps, typename = std::enable_if_t<all_base_of_v<Component, Comps...>>>
Archetype* CreateArchetypeFrom()
{
	//if constexpr(sizeof...(Comps) != 0)
	{
		static const ComponentType* compTypes[] = { GetTypeFor<Comps>()... };
		constexpr size_t typeCount = ArraySize(compTypes);

		InsertionSort(compTypes);
		return CreateArchetypeFrom(compTypes, typeCount);
	}
}

}