

#include "Archetype.hpp"
#include "Entity.hpp"
#include "ComponentType.hpp"

namespace Musa
{

static forceinline uint8* GetComponentOffsetInChunk(ArchetypeChunk& chunk, size_t componentOffset)
{
	return reinterpret_cast<uint8*>(&chunk) + componentOffset;
}

static forceinline size_t GetComponentIndex(uint32 typeSize, uint32 compIndex)
{
	return typeSize * compIndex;
}

static forceinline void CheckIfFullChunk(ArchetypeChunk& chunk, uint32 capacity)
{
	if (chunk.footer.numEntities == capacity)
	{
		Archetype* owner = chunk.footer.owner;
		Assert(owner);
		++owner->fullChunkCount;
		SortChunksForFullness(*owner);
	}
}

static forceinline void CheckIfWasFullChunk(ArchetypeChunk& chunk, bool wasFull)
{
	if (wasFull)
	{
		Archetype* owner = chunk.footer.owner;
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
	for (uint32 i = startChunkIndex; i < entities.Size(); ++i)
	{
		Entity e = entities[i];
		EntityBridge& bridge = entityBridges[e.id];
		bridge.chunkIndex = i;
	}
}

static void FillGapInChunkEntities(ArchetypeChunk& chunk, uint32 entityIndex)
{
	uint8* chunkBuffer = reinterpret_cast<uint8*>(&chunk);
	
	// TODO - Cache miss!
	// NOTE - This works because numEntities already accounts for the removed entity
	uint32 entitiesToMove = chunk.footer.numEntities - entityIndex;

	if (entitiesToMove > 0)
	{
		// Move Entity array over starting with entityIndex + 1
		uint8* dstEntityLoc = chunkBuffer + sizeof(Entity) * entityIndex;
		uint8* srcEntityLoc = dstEntityLoc + sizeof(Entity);
		size_t moveSize = entitiesToMove * sizeof(Entity);
		Memcpy(dstEntityLoc, srcEntityLoc, moveSize);

		// Move Components over starting with entityIndex + 1
		auto& offsets = *chunk.footer.offsets;
		auto& types = *chunk.footer.types;
		const uint32 numComponents = types.Size();
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
	ArchetypeComponentList& oldTypes,
	ArchetypeComponentList& newTypes,
	uint32 oldCount, uint32 newCount
)
{
	uint32 sameTypeCount = 0;
	uint32 newTypeIndex = 0;
	for (uint32 oldTypeIndex = 0;
		oldTypeIndex < oldCount && newTypeIndex < newCount;
		++oldTypeIndex, ++newTypeIndex)
	{
		const ComponentType* oldType = oldTypes[oldTypeIndex];
		Assert(oldType->size != 0);
		const ComponentType* newType = newTypes[newTypeIndex];

		while (oldType != newType &&
			newTypeIndex < newCount)
		{
			++newTypeIndex;
			newType = newTypes[newTypeIndex];
		}

		if (oldType == newType)
		{
			typeIndexArray[sameTypeCount].oldIndex = oldTypeIndex;
			typeIndexArray[sameTypeCount].newIndex = newTypeIndex;
			typeIndexArray[sameTypeCount].size = oldType->size;
			++sameTypeCount;
		}
	}

	return sameTypeCount;
}

void ConstructEntityInChunk(ArchetypeChunk& chunk, uint32 entityIndex)
{
	Assert(chunk.footer.numEntities > entityIndex);
	auto& offsetList = *chunk.footer.offsets;
	auto& typeList = *chunk.footer.types;
	// Initialize memory using contrustor
	for(uint32 i = 0; i < typeList.Size(); ++i)
	//for (const auto& compOffset : *offsetList)
	{
		const ComponentType& compType = *typeList[i];
		size_t compOffset = offsetList[i];

		// TODO - Support empty classes because of tagging
		Assert(compType.size > 0);
		void* compAddr = GetComponentOffsetInChunk(chunk, compOffset) + GetComponentIndex(compType.size, entityIndex);
		compType.ctor(compAddr);
	}
}

void DestructEntityInChunk(ArchetypeChunk& chunk, uint32 entityIndex)
{
	Assert(chunk.footer.numEntities > entityIndex);
	auto& offsetList = *chunk.footer.offsets;
	auto& typeList = *chunk.footer.types;

	// Deinitialize memory using destrustor
	for (uint32 i = 0; i < typeList.Size(); ++i)
	{
		const ComponentType& compType = *typeList[i];
		size_t compOffset = offsetList[i];

		// TODO - Support empty classes because of tagging
		Assert(compType.size > 0);
		void* compAddr = GetComponentOffsetInChunk(chunk, compOffset) + GetComponentIndex(compType.size, entityIndex);
		compType.dtor(compAddr);
	}
}

// Returns the new chunk index
uint32 MoveEntityToChunk(Entity& entity, ArchetypeChunk& oldChunk, uint32 oldChunkIndex, ArchetypeChunk& newChunk)
{
	uint32 oldComponentCount = oldChunk.footer.offsets->Size();
	uint32 newComponentCount = newChunk.footer.offsets->Size();
	auto& oldTypesList = oldChunk.footer.types;
	auto& newTypesList = newChunk.footer.types;
	auto& oldOffsets = *oldChunk.footer.offsets;
	auto& newOffsets = *newChunk.footer.offsets;

	uint32 newChunkIndex = AddEntityToChunk(newChunk, entity);


	//copy all data from old chunk into new chunk
	//bad iteration, fix later

	SameComponentType sameTypes[MaxComponentsPerArchetype];

	uint32 sameTypeCount = AssociateSameComponentTypes(
		sameTypes,
		*oldTypesList, 
		*newTypesList, 
		oldComponentCount, 
		newComponentCount);

	for (uint32 i = 0; i < sameTypeCount; ++i)
	{
		uint32 oldIndex = sameTypes[i].oldIndex;
		uint32 newIndex = sameTypes[i].newIndex;
		uint32 compSize = sameTypes[i].size;
		
		void* oldAddr = GetComponentOffsetInChunk(oldChunk, oldOffsets[oldIndex]) + GetComponentIndex(compSize, oldChunkIndex);
		void* newAddr = GetComponentOffsetInChunk(newChunk, newOffsets[newIndex]) + GetComponentIndex(compSize, newChunkIndex);

		Memcpy(newAddr, compSize, oldAddr, compSize);
	}

	return newChunkIndex;
}

uint32 AddEntityToChunk(ArchetypeChunk& chunk, const Entity& entity)
{
	REF_CHECK(chunk, entity);

	uint32 archetypeCapacity = chunk.footer.owner->entityCapacity;
	Assert(chunk.footer.numEntities < archetypeCapacity);

	uint32 entityIndex = chunk.footer.numEntities;
	++chunk.footer.numEntities;

	Entity* entityArray = reinterpret_cast<Entity*>(&chunk);
	new(entityArray + entityIndex) Entity{ entity };

	CheckIfFullChunk(chunk, archetypeCapacity);

	return entityIndex;
}
void RemoveEntityFromChunk(ArchetypeChunk& chunk, uint32 chunkIndex)
{
	Assert(chunk.footer.numEntities > chunkIndex);

	bool fullBeforeRemove = chunk.footer.numEntities == chunk.footer.owner->entityCapacity;

	DestructEntityInChunk(chunk, chunkIndex);
	--chunk.footer.numEntities;

	FillGapInChunkEntities(chunk, chunkIndex);
	ReconcileEntityChunkChanges(*chunk.footer.owner->world, chunk, chunkIndex);

	CheckIfWasFullChunk(chunk, fullBeforeRemove);
}
}

