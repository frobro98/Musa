
#include "World.hpp"
#include "ArchetypeChunk.hpp"
#include "Archetype.hpp"

namespace Musa
{

static void ClearEntityBridge(World& world, Entity entity)
{
	world.deadIndices.Add(entity.id);
	++world.entityBridges[entity.id].version;
	world.entityBridges[entity.id].chunk = nullptr;
	world.entityBridges[entity.id].chunkIndex = 0;

	--world.totalLivingEntities;
}

//////////////////////////////////////////////////////////////////////////

Entity World::ConstructEntityInternals(World& world, const ComponentType** types, uint32 typeCount)
{
	Assert(IsSorted(types, typeCount, Less<const ComponentType*>{}));
	Archetype* archetype = GetOrCreateArchetypeFrom(world, types, typeCount);
	Assert(archetype);

	Internal::CheckForSameComponents(types, typeCount);

	return CreateEntityWith(*archetype);
}

void World::HookUpComponentType(World& world, Entity entity, const ComponentType* type)
{
	const ComponentType* componentTypes[MaxComponentsPerArchetype];
	Archetype& currentArchetype = GetEntityArchetype(world, entity);
	ArchetypeComponentList& currentTypes = currentArchetype.types;

	uint32 totalTypes = currentTypes.Size();
	Assert(totalTypes < MaxComponentsPerArchetype);

	// Check if type is already on current archetype
	for (uint32 i = 0; i < totalTypes; ++i)
	{
		componentTypes[i] = currentTypes[i];
	}

	componentTypes[totalTypes] = type;
	++totalTypes;

	InsertionSort(componentTypes, totalTypes);

	Internal::CheckForSameComponents(componentTypes, totalTypes);

	Archetype* newArchetype = GetOrCreateArchetypeFrom(world, componentTypes, totalTypes);

	SetEntitysArchetype(world, entity, *newArchetype);

}

void World::UnhookComponentType(World& world, Entity entity, const ComponentType* type)
{
	const ComponentType* componentTypes[MaxComponentsPerArchetype];
	Archetype& currentArchetype = GetEntityArchetype(world, entity);
	ArchetypeComponentList& currentTypes = currentArchetype.types;

	uint32 totalTypes = currentTypes.Size();
	Assert(totalTypes < MaxComponentsPerArchetype);

	bool typeFound = false;
	// Check if type is already on current archetype
	for (uint32 i = 0; i < totalTypes; ++i)
	{
		componentTypes[i] = currentTypes[i];

		if (componentTypes[i] == type)
		{
			typeFound = true;
			componentTypes[i] = currentTypes[totalTypes - 1];
		}
	}

	if (typeFound)
	{
		--totalTypes;
		InsertionSort(componentTypes, totalTypes);

		Archetype* newArchetype = GetOrCreateArchetypeFrom(world, componentTypes, totalTypes);

		SetEntitysArchetype(world, entity, *newArchetype);
	}
}

World::World()
	: queryCache(new QueryCache(*this))
{
}

Entity World::CreateEntity(const ComponentType ** types, uint32 typeCount)
{
	Assert(typeCount < MaxComponentsPerArchetype);
	InsertionSort(types, typeCount);
	return ConstructEntityInternals(*this, types, typeCount);
}

Entity World::CreateEntity(Archetype& archetype)
{
	return CreateEntityWith(archetype);
}

void World::DestroyEntity(Entity entity)
{
	Assert(IsEntityValid(entity));
	RemoveEntityFromChunk(*entityBridges[entity.id].chunk, entityBridges[entity.id].chunkIndex);
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
void World::Update()
{
	// Sort systems if necessary

	// Update all systems
	for (auto& system : systems)
	{
		system->Update();
		ResetInternalCache();
	}
}
}

