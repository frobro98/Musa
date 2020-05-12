// Copyright 2020, Nathan Blane

#include "Archetype.hpp"
#include "Entity.hpp"
#include "ComponentType.hpp"
#include "ECS/World.hpp"

namespace Musa
{

static forceinline size_t GetComponentIndex(uint32 typeSize, uint32 compIndex)
{
	return typeSize * compIndex;
}

static forceinline void CheckIfFullChunk(ArchetypeChunk& chunk, uint32 capacity)
{
	if (chunk.header->entityCount == capacity)
	{
		Archetype* owner = chunk.header->archetype;
		Assert(owner);
		++owner->fullChunkCount;
		SortChunksForFullness(*owner);
	}
}

static forceinline void CheckIfWasFullChunk(ArchetypeChunk& chunk, bool wasFull)
{
	if (wasFull)
	{
		Archetype* owner = chunk.header->archetype;
		Assert(owner);
		--owner->fullChunkCount;
		SortChunksForFullness(*owner);
	}
}

// Fixes Entity indices into their owning chunk
static void ReconcileEntityChunkChanges(World& world, ArchetypeChunk& changedChunk, uint32 startChunkIndex)
{
	DynamicArray<EntityBridge>& entityBridges = world.entityBridges;
	ChunkArray<Entity> entities = GetChunkArray<Entity>(changedChunk);
	Assert(entities.IsValid());

	for (uint32 i = startChunkIndex; i < entities.size; ++i)
	{
		Entity e = entities[i];
		EntityBridge& bridge = entityBridges[e.id];
		bridge.chunkIndex = i;
	}
}

static void FillGapInChunkEntities(ArchetypeChunk& chunk, uint32 entityIndex)
{
	uint8* chunkBuffer = chunk.data;
	
	// TODO - Cache miss!
	// NOTE - This works because numEntities already accounts for the removed entity
	uint32 entitiesToMove = chunk.header->entityCount - entityIndex;

	if (entitiesToMove > 0)
	{
		// Move Entity array over starting with entityIndex + 1
		uint8* dstEntityLoc = chunkBuffer + sizeof(Entity) * entityIndex;
		uint8* srcEntityLoc = dstEntityLoc + sizeof(Entity);
		size_t moveSize = entitiesToMove * sizeof(Entity);
		Memcpy(dstEntityLoc, srcEntityLoc, moveSize);

		// Move Components over starting with entityIndex + 1
		size_t* offsets = chunk.header->offsets;
		const ComponentType** types = chunk.header->types;
		const uint32 numComponents = chunk.header->componentTypeCount;
		for (uint32 i = 0; i < numComponents; ++i)
		{
			size_t offset = offsets[i];
			uint32 size = types[i]->size;
			uint8* dstCompLoc = chunkBuffer + offset + (size * entityIndex);
			uint8* srcCompLoc = dstCompLoc + size;
			size_t compMoveSize = entitiesToMove * size;

			Memcpy(dstCompLoc, srcCompLoc, compMoveSize);
		}
	}
}


namespace
{
struct SameComponentType {
	uint32 size;
	uint32 oldIndex;
	uint32 newIndex;
};
}

// Returns how many types matched up
static uint32 AssociateSameComponentTypes(
	SameComponentType* typeIndexArray,
	const ComponentType** oldTypes,
	const ComponentType** newTypes,
	uint32 oldCount, uint32 newCount
)
{
	uint32 sameTypeCount = 0;

	uint32 biggerTypeIndex = 0;
	const uint32 smallCount = /*Math::Max()*/ oldCount < newCount ? oldCount : newCount;
	const uint32 biggerCount = /*Math::Max()*/ oldCount > newCount ? oldCount : newCount;
	const ComponentType** smallList = /*Math::Select()*/ oldCount < newCount ? oldTypes : newTypes;
	const ComponentType** biggerList = /*Math::Select()*/ oldCount > newCount ? oldTypes : newTypes;
	for (uint32 smallTypeIndex = 0;
		smallTypeIndex < smallCount && biggerTypeIndex < biggerCount;
		++smallTypeIndex, ++biggerTypeIndex)
	{
		const ComponentType* smallType = smallList[smallTypeIndex];
		Assert(smallType->size != 0);
		const ComponentType* biggerType = biggerList[biggerTypeIndex];

		while (smallType != biggerType &&
			biggerTypeIndex < biggerCount-1)
		{
			++biggerTypeIndex;
			biggerType = biggerList[biggerTypeIndex];
		}

		if (smallType == biggerType)
		{
			typeIndexArray[sameTypeCount].oldIndex = oldCount < newCount ? smallTypeIndex : biggerTypeIndex;
			typeIndexArray[sameTypeCount].newIndex = newCount < oldCount ? smallTypeIndex : biggerTypeIndex;
			typeIndexArray[sameTypeCount].size = smallType->size;
			++sameTypeCount;
		}
	}

	return sameTypeCount;
}

void ConstructEntityInChunk(ArchetypeChunk& chunk, uint32 entityIndex)
{
	Assert(chunk.header->entityCount > entityIndex);
	size_t* offsets= chunk.header->offsets;
	const ComponentType** types = chunk.header->types;
	uint32 typeCount = chunk.header->componentTypeCount;

	// Initialize memory using construstor
	for(uint32 i = 0; i < typeCount; ++i)
	//for (const auto& compOffset : *offsetList)
	{
		const ComponentType* compType = types[i];
		size_t componentOffset = offsets [i];

		// TODO - Support empty classes because of tagging
		Assert(compType->size > 0);
		void* compAddr = chunk.data + componentOffset + GetComponentIndex(compType->size, entityIndex);
		compType->ctor(compAddr);
	}
}

void DestructEntityInChunk(ArchetypeChunk& chunk, uint32 entityIndex)
{
	Assert(chunk.header->entityCount > entityIndex);
	size_t* offsets = chunk.header->offsets;
	const ComponentType** types = chunk.header->types;
	uint32 typeCount = chunk.header->componentTypeCount;

	// Deinitialize memory using destrustor
	for (uint32 i = 0; i < typeCount; ++i)
	{
		const ComponentType* compType = types[i];
		size_t componentOffset = offsets[i];

		// TODO - Support empty classes because of tagging
		Assert(compType->size > 0);
		void* compAddr = chunk.data + componentOffset + GetComponentIndex(compType->size, entityIndex);
		compType->dtor(compAddr);
	}
}

// Returns the new chunk index
uint32 MoveEntityToChunk(Entity& entity, ArchetypeChunk& oldChunk, uint32 oldChunkIndex, ArchetypeChunk& newChunk)
{
	uint32 oldComponentCount = oldChunk.header->componentTypeCount;
	uint32 newComponentCount = newChunk.header->componentTypeCount;
	const ComponentType** oldTypes = oldChunk.header->types;
	const ComponentType** newTypes = newChunk.header->types;
	size_t* oldOffsets = oldChunk.header->offsets;
	size_t* newOffsets = newChunk.header->offsets;

	uint32 newChunkIndex = AddEntityToChunk(newChunk, entity);


	//copy all data from old chunk into new chunk
	//bad iteration, fix later

	SameComponentType sameTypes[MaxComponentsPerArchetype];

	uint32 sameTypeCount = AssociateSameComponentTypes(
		sameTypes,
		oldTypes, 
		newTypes, 
		oldComponentCount, 
		newComponentCount);

	for (uint32 i = 0; i < sameTypeCount; ++i)
	{
		uint32 oldIndex = sameTypes[i].oldIndex;
		uint32 newIndex = sameTypes[i].newIndex;
		uint32 compSize = sameTypes[i].size;
		
		void* oldAddr = oldChunk.data + oldOffsets[oldIndex] + GetComponentIndex(compSize, oldChunkIndex);
		void* newAddr = newChunk.data + newOffsets[newIndex] + GetComponentIndex(compSize, newChunkIndex);

		Memcpy(newAddr, compSize, oldAddr, compSize);
	}

	return newChunkIndex;
}

uint32 AddEntityToChunk(ArchetypeChunk& chunk, const Entity& entity)
{
	REF_CHECK(chunk, entity);

	uint32 archetypeCapacity = chunk.header->archetype->entityCapacity;
	Assert(chunk.header->entityCount < archetypeCapacity);

	uint32 entityIndex = chunk.header->entityCount;
	++chunk.header->entityCount;
	++chunk.header->archetype->totalEntityCount;

	Entity* entityArray = reinterpret_cast<Entity*>(chunk.data);
	new(entityArray + entityIndex) Entity{ entity };

	CheckIfFullChunk(chunk, archetypeCapacity);

	return entityIndex;
}
void RemoveEntityFromChunk(ArchetypeChunk& chunk, uint32 chunkIndex)
{
	Assert(chunk.header->entityCount > chunkIndex);

	bool fullBeforeRemove = chunk.header->entityCount == chunk.header->archetype->entityCapacity;

	DestructEntityInChunk(chunk, chunkIndex);
	--chunk.header->entityCount;
	--chunk.header->archetype->totalEntityCount;

	FillGapInChunkEntities(chunk, chunkIndex);
	ReconcileEntityChunkChanges(*chunk.header->archetype->world, chunk, chunkIndex);

	CheckIfWasFullChunk(chunk, fullBeforeRemove);
}
}

