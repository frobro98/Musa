#pragma once

#include "EngineCore/Algorithms.hpp"
#include "Containers/DynamicArray.hpp"
#include "Containers/StaticArray.hpp"
#include "Utilities/TemplateUtils.hpp"
#include "ECS/Types.hpp"
#include "ECS/DLLDef.h"
#include "ECS/ComponentType.hpp"
#include "ECS/ArchetypeChunk.hpp"


namespace Musa
{
class World;

namespace Internal
{
// TODO - This should be in some sort of Utility for ComponentTypes
static forceinline void CheckForSameComponents(const ComponentType** types, size_t typeCount)
{
	Assert(typeCount < MaxComponentsPerArchetype);
	Assert(IsSorted(types, typeCount, Less<const ComponentType*>{}));

	NOT_USED auto checkTypes = [types, typeCount] {
		// Check if type is already on current archetype
		for (uint32 i = 1; i < typeCount; ++i)
		{
			if (types[i] == types[i - 1])
			{
				return false;
			}
		}
		return true;
	};

	AssertFunc(checkTypes, "There are duplicate components!");
}

}

using ArchetypeComponentList = DynamicArray<const ComponentType*>;
using ArchetypeComponentHashList = DynamicArray<ComponentTypeHash>;
using ArchetypeComponentOffsetList = DynamicArray<size_t>;

// TODO - Consider different memory layout schemes for archetypes and chunks
// Contains blocks of memory which contain the actual component data
// Similar to how Unity ECS archetypes are implemented
struct Archetype
{
	World* world;
	// Must have some sort of identifier which also shows what components are stored within it
	// TODO - This will result in leaks potentially. The hope will be to store chunks in the same contiguous memory, so that'll relieve some of the leak maybe?
	DynamicArray<ArchetypeChunk> chunks;
	
	// SOA!
	// TODO - Consider breaking out more information from the types into other arrays
	ArchetypeComponentList types;
	ArchetypeComponentHashList typeHashes;
	ArchetypeComponentOffsetList offsets;

	ArchetypeMask archetypeMask;
	uint32 totalEntityCount;
	uint32 entityCapacity;
	uint32 fullChunkCount;
};

ECS_API Archetype* GetOrCreateArchetypeFrom(World& world, const ComponentType** compTypes, size_t typeCount);

template<typename... Comps, typename = std::enable_if_t<all_base_of_v<Component, Comps...>>>
ECS_TEMPLATE Archetype* GetOrCreateArchetypeFrom(World& world)
{
	REF_CHECK(world);

	if constexpr (sizeof...(Comps) > 0)
	{
		static const ComponentType* compTypes[] = { GetComponentTypeFor<Comps>()... };
		constexpr size_t typeCount = ArraySize(compTypes);

		InsertionSort(compTypes, typeCount);
		// NOTE - No chunk created at this point because there isn't any reason for it to be. It needs to be created when an Entity is added...
		return GetOrCreateArchetypeFrom(world, compTypes, typeCount);
	}
	else
	{
		return GetOrCreateArchetypeFrom(world, nullptr, 0);

	}
}

ArchetypeChunk GetOrCreateFreeArchetypeChunk(Archetype& archetype);

// TODO - This isn't really that great of a name I think, and it shouldn't be this public
void SetEntitysArchetype(World& world, Entity entity, Archetype& archetype);

void SortChunksForFullness(Archetype& archetype);

}