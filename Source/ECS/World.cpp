
#include "World.hpp"
#include "ArchetypeChunk.hpp"
#include "Archetype.hpp"

#include "ECS/ComponentTypeOffsetList.hpp"

namespace Musa
{

static void ClearEntityBridge(World& world, Entity entity)
{
	world.deadIndices.Add(entity.id);
	++world.entityBridges[entity.id].version;
	world.entityBridges[entity.id].owningChunk = nullptr;
	world.entityBridges[entity.id].chunkIndex = 0;

	--world.totalLivingEntities;
}

//////////////////////////////////////////////////////////////////////////

Entity World::ConstructEntityInternals(World& world, const ComponentType** types, uint32 typeCount)
{
	Assert(IsSorted(types, typeCount, Less<const ComponentType*>{}));
	Archetype* archetype = GetOrCreateArchetypeFrom(world, types, typeCount);
	Assert(archetype);

	return CreateEntityWith(*archetype);
}


void World::HookUpComponentType(World& world, Entity entity, const ComponentType* type)
{
	const ComponentType* componentTypes[MaxComponentsPerArchetype];
	Archetype& currentArchetype = GetEntityArchetype(world, entity);
	ComponentTypeOffsetList& currentOffsets = currentArchetype.offsetList;

	uint32 totalTypes = currentOffsets.offsets.Size();
	Assert(totalTypes < MaxComponentsPerArchetype);

	bool typeFound = false;
	// Check if type is already on current archetype
	for (uint32 i = 0; i < currentOffsets.offsets.Size(); ++i)
	{
		componentTypes[i] = currentOffsets.offsets[i].type;

		if (componentTypes[i] == type)
		{
			typeFound = true;
		}
	}

	if (!typeFound)
	{
		componentTypes[totalTypes] = type;
		++totalTypes;
		InsertionSort(componentTypes, totalTypes);

		Archetype* newArchetype = GetOrCreateArchetypeFrom(world, componentTypes, totalTypes);

		SetEntitysArchetype(world, entity, *newArchetype);
	}
}

void World::UnhookComponentType(World& world, Entity entity, const ComponentType* type)
{
	Assert(false);
	const ComponentType* componentTypes[MaxComponentsPerArchetype];
	Archetype& currentArchetype = GetEntityArchetype(world, entity);
	ComponentTypeOffsetList& currentOffsets = currentArchetype.offsetList;

	uint32 totalTypes = currentOffsets.offsets.Size();
	Assert(totalTypes < MaxComponentsPerArchetype);

	bool typeFound = false;
	// Check if type is already on current archetype
	for (uint32 i = 0; i < currentOffsets.offsets.Size(); ++i)
	{
		componentTypes[i] = currentOffsets.offsets[i].type;

		if (componentTypes[i] == type)
		{
			typeFound = true;

		}
	}

	if (!typeFound)
	{
		componentTypes[totalTypes] = type;
		InsertionSort(componentTypes);

		Archetype* newArchetype = GetOrCreateArchetypeFrom(world, componentTypes, totalTypes + 1);

		SetEntitysArchetype(world, entity, *newArchetype);
	}
}

void World::DestroyEntity(Entity entity)
{
	Assert(IsEntityValid(entity));
	RemoveEntityFromChunk(*entityBridges[entity.id].owningChunk, entityBridges[entity.id].chunkIndex);
	ClearEntityBridge(*this, entity);
}
bool World::IsEntityValid(Entity entity) const
{
	if (entityBridges.Size() > entity.id)
	{
		if (entity.version != 0 && entityBridges[entity.id].version == entity.version)
		{
			return true;
		}
	}

	return false;
}
}

