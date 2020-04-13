#pragma once

#include "Types/Intrinsics.hpp"
#include "Containers/DynamicArray.hpp"
#include "ECS/Types.hpp"
#include "ECS/Internal/robin_hood.hpp"

namespace Musa
{

struct ArchetypeChunk;
struct Archetype;

struct EntityBridge
{
	ArchetypeChunk* owningChunk;
	uint32 version;
	//uint32 chunkIndex;
};

class World
{
public:


private:
	// stores all of the different archetypes based on their similar  archetypeHashIDs
	DynamicArray<EntityBridge> entityBridges;
	DynamicArray<uint32> deadIndices;

	DynamicArray<Archetype*> archetypes;
	DynamicArray<ArchetypeHashID> archetypeHashIDs;
	robin_hood::unordered_flat_map<ArchetypeHashID, DynamicArray<Archetype*>> archetypesByHash;
	uint32 totalLivingEntities = 0;
};

}

