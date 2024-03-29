// Copyright 2020, Nathan Blane

#include "EntityBridge.hpp"
#include "Entity.hpp"
#include "Archetype.hpp"
#include "World.hpp"

namespace Musa
{
static Entity DetermineNewEntity(World& world)
{
	++world.totalLivingEntities;

	if (world.deadIndices.Size() == 0)
	{
		EntityBridge bridge;
		bridge.chunk = EmptyChunk;
		bridge.version = 1;

		u32 id = world.entityBridges.Size();
		world.entityBridges.Add(bridge);

		return Entity{ id, bridge.version };
	}
	else
	{
		// Reuse dead entity bridge
		u32 deadIndex = world.deadIndices.Last();
		world.deadIndices.RemoveLastOf(deadIndex);

		u32 version = world.entityBridges[deadIndex].version;
		u32 id = deadIndex;

		return Entity{ id, version };
	}
}

Entity CreateEntityWith(Archetype& archetype)
{
	World& world = *archetype.world;

	Entity entity = DetermineNewEntity(world);

	SetEntitysArchetype(world, entity, archetype);

	return entity;
}
}