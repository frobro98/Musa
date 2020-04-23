#pragma once

#include "EngineCore/Algorithms.hpp"
#include "EngineCore/MemoryUtilities.h"
#include "Containers/DynamicArray.hpp"
#include "Containers/StaticArray.hpp"
#include "Utilities/TemplateUtils.hpp"
#include "ECS/Types.hpp"
#include "ECS/ComponentType.hpp"
#include "ECS/ComponentTypeOffsetList.hpp"
#include "ECS/ArchetypeChunk.hpp"
#include "ECS/World.hpp"


namespace Musa
{
constexpr uint32 MaxComponentsPerArchetype = 32;

// Contains blocks of memory which contain the actual component data
// Similar to how Unity ECS archetypes are implemented
struct Archetype
{
	World* world;
	// Must have some sort of identifier which also shows what components are stored within it
	DynamicArray<UniquePtr<ArchetypeChunk>> chunkList;
	ComponentTypeOffsetList offsetList;
	ArchetypeHashID archetypeHashID;
	uint32 fullChunks;
};

ECS_API Archetype* GetOrCreateArchetypeFrom(World& world, const ComponentType** compTypes, size_t typeCount);

template<typename... Comps, typename = std::enable_if_t<all_base_of_v<Component, Comps...>>>
ECS_TEMPLATE Archetype* GetOrCreateArchetypeFrom(World& world)
{
	REF_CHECK(world);

	//if constexpr(sizeof...(Comps) != 0)
	{
		static const ComponentType* compTypes[] = { GetTypeFor<Comps>()... };
		constexpr size_t typeCount = ArraySize(compTypes);

		InsertionSort(compTypes, typeCount);
		// NOTE - No chunk created at this point because there isn't any reason for it to be. It needs to be created when an Entity is added...
		return GetOrCreateArchetypeFrom(world, compTypes, typeCount);
	}
}

ArchetypeChunk& GetOrCreateFreeArchetypeChunk(Archetype& archetype);

forceinline Archetype& GetEntityArchetype(World& world, Entity entity)
{
	Assert(world.IsEntityValid(entity));
	return *world.entityBridges[entity.id].owningChunk->footer.owner;
}

// TODO - This isn't really that great of a name I think, and it shouldn't be this public
void SetEntitysArchetype(World& world, Entity entity, Archetype& archetype);

void SortChunksForFullness(Archetype& archetype);

}