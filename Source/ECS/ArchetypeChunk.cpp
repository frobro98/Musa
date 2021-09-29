// Copyright 2020, Nathan Blane

#include "Archetype.hpp"
#include "Entity.hpp"
#include "ComponentType.hpp"
#include "ECS/World.hpp"

namespace Musa
{

static forceinline size_t GetComponentIndex(u32 typeSize, u32 compIndex)
{
	return typeSize * compIndex;
}

static forceinline void CheckIfFullChunk(ArchetypeChunk& chunk, u32 capacity)
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

static forceinline void ValidateChunk(ArchetypeChunk& chunk)
{
	Entity* entityArr = reinterpret_cast<Entity*>(chunk.data);
	for (u32 i = 0; i < chunk.header->entityCount; ++i)
	{
		Entity e0 = entityArr[i];
		for (u32 j = i + 1; j < chunk.header->entityCount; ++j)
		{
			Entity e1 = entityArr[j];
			Assert(e0.id != e1.id);
		}
	}
}

// Fixes Entity indices into their owning chunk
static void ReconcileEntityChunkChanges(World& world, ArchetypeChunk& changedChunk, u32 startChunkIndex)
{
	DynamicArray<EntityBridge>& entityBridges = world.entityBridges;
	ChunkArray<Entity> entities = GetChunkArray<Entity>(changedChunk);
	Assert(entities.IsValid());

	for (u32 i = startChunkIndex; i < entities.size; ++i)
	{
		Entity e = entities[i];
		EntityBridge& bridge = entityBridges[e.id];
		bridge.chunkIndex = i;
	}
}

static void FillGapInChunkEntities(ArchetypeChunk& chunk, u32 entityIndex)
{
	u8* chunkBuffer = chunk.data;
	
	// TODO - Cache miss!
	// NOTE - This works because numEntities already accounts for the removed entity
	u32 entitiesToMove = chunk.header->entityCount - entityIndex;

	if (entitiesToMove > 0)
	{
		// Move Entity array over starting with entityIndex + 1
		u8* dstEntityLoc = chunkBuffer + sizeof(Entity) * entityIndex;
		u8* srcEntityLoc = dstEntityLoc + sizeof(Entity);
		size_t moveSize = entitiesToMove * sizeof(Entity);
		Memcpy(dstEntityLoc, srcEntityLoc, moveSize);

		// Move Components over starting with entityIndex + 1
		size_t* offsets = chunk.header->offsets;
		const ComponentType** types = chunk.header->types;
		const u32 numComponents = chunk.header->componentTypeCount;
		for (u32 i = 0; i < numComponents; ++i)
		{
			u32 size = types[i]->size;
			if (size > 0)
			{
				size_t offset = offsets[i];
				u8* dstCompLoc = chunkBuffer + offset + (size * entityIndex);
				u8* srcCompLoc = dstCompLoc + size;
				size_t compMoveSize = entitiesToMove * size;

				Memcpy(dstCompLoc, srcCompLoc, compMoveSize);
			}
		}
	}
}


namespace
{
struct SameComponentType {
	u32 size;
	u32 oldIndex;
	u32 newIndex;
};
}

// Returns how many types matched up
static u32 AssociateSameComponentTypes(
	SameComponentType* typeIndexArray,
	const ComponentType** oldTypes,
	const ComponentType** newTypes,
	u32 oldCount, u32 newCount
)
{
	u32 sameTypeCount = 0;

	u32 biggerTypeIndex = 0;
	const u32 smallCount = /*Math::Max()*/ oldCount < newCount ? oldCount : newCount;
	const u32 biggerCount = /*Math::Max()*/ oldCount > newCount ? oldCount : newCount;
	const ComponentType** smallList = /*Math::Select()*/ oldCount < newCount ? oldTypes : newTypes;
	const ComponentType** biggerList = /*Math::Select()*/ oldCount > newCount ? oldTypes : newTypes;
	for (u32 smallTypeIndex = 0;
		smallTypeIndex < smallCount && biggerTypeIndex < biggerCount;
		++smallTypeIndex, ++biggerTypeIndex)
	{
		const ComponentType* smallType = smallList[smallTypeIndex];
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

void ConstructEntityInChunk(ArchetypeChunk& chunk, u32 entityIndex)
{
	Assert(chunk.header->entityCount > entityIndex);
	size_t* offsets= chunk.header->offsets;
	const ComponentType** types = chunk.header->types;
	u32 typeCount = chunk.header->componentTypeCount;

	// Initialize memory using construstor
	for(u32 i = 0; i < typeCount; ++i)
	{
		const ComponentType* compType = types[i];
		size_t componentOffset = offsets [i];

		// TODO - Figure out if component size being non-zero is actually likely or not
		if (likely(compType->size > 0))
		{
			void* compAddr = chunk.data + componentOffset + GetComponentIndex(compType->size, entityIndex);
			compType->ctor(compAddr);
		}
	}
}

void DestructEntityInChunk(ArchetypeChunk& chunk, u32 entityIndex)
{
	Assert(chunk.header->entityCount > entityIndex);
	size_t* offsets = chunk.header->offsets;
	const ComponentType** types = chunk.header->types;
	u32 typeCount = chunk.header->componentTypeCount;

	// Deinitialize memory using destrustor
	for (u32 i = 0; i < typeCount; ++i)
	{
		const ComponentType* compType = types[i];
		size_t componentOffset = offsets[i];

		// TODO - Figure out if component size being non-zero is actually likely or not
		if (likely(compType->size > 0))
		{
			void* compAddr = chunk.data + componentOffset + GetComponentIndex(compType->size, entityIndex);
			compType->dtor(compAddr);
		}
	}
}

// Returns the new chunk index
u32 MoveEntityToChunk(Entity& entity, ArchetypeChunk& oldChunk, u32 oldChunkIndex, ArchetypeChunk& newChunk)
{
	u32 oldComponentCount = oldChunk.header->componentTypeCount;
	u32 newComponentCount = newChunk.header->componentTypeCount;
	const ComponentType** oldTypes = oldChunk.header->types;
	const ComponentType** newTypes = newChunk.header->types;
	size_t* oldOffsets = oldChunk.header->offsets;
	size_t* newOffsets = newChunk.header->offsets;

	u32 newChunkIndex = AddEntityToChunk(newChunk, entity);


	//copy all data from old chunk into new chunk
	//bad iteration, fix later

	SameComponentType sameTypes[MaxComponentsPerArchetype];

	u32 sameTypeCount = AssociateSameComponentTypes(
		sameTypes,
		oldTypes, 
		newTypes, 
		oldComponentCount, 
		newComponentCount);

	for (u32 i = 0; i < sameTypeCount; ++i)
	{
		u32 compSize = sameTypes[i].size;
		if (compSize > 0)
		{
			u32 oldIndex = sameTypes[i].oldIndex;
			u32 newIndex = sameTypes[i].newIndex;

			void* oldAddr = oldChunk.data + oldOffsets[oldIndex] + GetComponentIndex(compSize, oldChunkIndex);
			void* newAddr = newChunk.data + newOffsets[newIndex] + GetComponentIndex(compSize, newChunkIndex);

			Memcpy(newAddr, compSize, oldAddr, compSize);
		}
	}

	return newChunkIndex;
}

u32 AddEntityToChunk(ArchetypeChunk& chunk, const Entity& entity)
{
	REF_CHECK(chunk, entity);

	u32 archetypeCapacity = chunk.header->archetype->entityCapacity;
	Assert(chunk.header->entityCount < archetypeCapacity);

	u32 entityIndex = chunk.header->entityCount;
	++chunk.header->entityCount;
	++chunk.header->archetype->totalEntityCount;

	Entity* entityArray = reinterpret_cast<Entity*>(chunk.data);
	new(entityArray + entityIndex) Entity{ entity };

	CheckIfFullChunk(chunk, archetypeCapacity);
	ValidateChunk(chunk);

	return entityIndex;
}
void RemoveEntityFromChunk(ArchetypeChunk& chunk, u32 chunkIndex)
{
	Assert(chunk.header->entityCount > chunkIndex);

	bool fullBeforeRemove = chunk.header->entityCount == chunk.header->archetype->entityCapacity;

	// TODO - There needs to be some form of destruction for components that are on a destroyed entity
	//DestructEntityInChunk(chunk, chunkIndex);
	--chunk.header->entityCount;
	--chunk.header->archetype->totalEntityCount;

	FillGapInChunkEntities(chunk, chunkIndex);
	ReconcileEntityChunkChanges(*chunk.header->archetype->world, chunk, chunkIndex);

	CheckIfWasFullChunk(chunk, fullBeforeRemove);
	ValidateChunk(chunk);
}
}

