// Copyright 2020, Nathan Blane

#pragma once

#include "Algorithms/Algorithms.hpp"
#include "Containers/DynamicArray.hpp"
#include "Containers/StaticArray.hpp"
#include "Utilities/TemplateUtils.hpp"
#include "Utilities/Array.hpp"
#include "ECS/Types.hpp"
#include "ECS/ECSAPI.hpp"
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
		for (u32 i = 1; i < typeCount; ++i)
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
	u32 totalEntityCount;
	u32 entityCapacity;
	u32 fullChunkCount;
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

NODISCARD ECS_API u32 GetArchetypeSystemVersion(const Archetype& archetype);

// TODO - Consider not having this function exist, now that there's the ChunkComponentAccessor. The one issue would be World::SetComponentDataOn
template<typename Comp>
NODISCARD ECS_TEMPLATE forceinline ChunkArray<Comp> GetChunkArray(ArchetypeChunk& chunk)
{
	if constexpr (std::is_same_v<Comp, Entity>)
	{
		Entity* entityArr = reinterpret_cast<Entity*>(chunk.data);
		return ChunkArray<Entity>(*entityArr, chunk.header->entityCount);
	}
	else
	{
		static_assert(is_valid_component_type_v<Comp>,
			"Template type parameter doesn't meet the requirements of being a component!");

		using sanitizedType = std::remove_reference_t<std::remove_const_t<Comp>>;
		using nonRefType = std::remove_reference_t<Comp>;

		constexpr bool isConst = std::is_const_v<Comp>;
		constexpr u64 hash = Musa::Internal::TypenameHash<sanitizedType>();
		ComponentTypeHash* hashes = chunk.header->typeHashes;
		const u32 typeCount = chunk.header->componentTypeCount;

		if (typeCount > 0)
		{
			for (u32 i = 0; i < typeCount; ++i)
			{
				if (hashes[i].typenameHash == hash)
				{
					if constexpr (!isConst)
					{
						// TODO - This is awful and needs to change. Might need to pass in the system or world or something, but it needs to change
						chunk.header->versions[i] = GetArchetypeSystemVersion(*chunk.header->archetype);
					}

					size_t* offsets = chunk.header->offsets;
					nonRefType* ptr = reinterpret_cast<nonRefType*>(chunk.data + offsets[i]);
					const u32 numEntities = chunk.header->entityCount;
					return ChunkArray<nonRefType>(*ptr, numEntities);
				}
			}
		}

		return ChunkArray<nonRefType>();
	}
}

ArchetypeChunk GetOrCreateFreeArchetypeChunk(Archetype& archetype);

// TODO - This isn't really that great of a name I think, and it shouldn't be this public
void SetEntitysArchetype(World& world, Entity entity, Archetype& archetype);

void SortChunksForFullness(Archetype& archetype);

}