

#include "Archetype.hpp"
#include "Entity.hpp"
#include "ComponentType.hpp"
#include "ComponentTypeOffsetList.hpp"

namespace Musa
{

static forceinline uint8* GetComponentOffsetInChunk(ArchetypeChunk& chunk, const ComponentTypeOffset& typeOffset)
{
	return reinterpret_cast<uint8*>(&chunk) + typeOffset.chunkOffset;
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
		++owner->fullChunks;
		SortChunksForFullness(*owner);
	}
}

static forceinline void CheckIfWasFullChunk(ArchetypeChunk& chunk, bool wasFull)
{
	if (wasFull)
	{
		Archetype* owner = chunk.footer.owner;
		Assert(owner);
		--owner->fullChunks;
		SortChunksForFullness(*owner);
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
	ComponentTypeOffsetList& oldOffsets,
	ComponentTypeOffsetList& newOffsets,
	uint32 oldCount, uint32 newCount
)
{
	uint32 sameTypeCount = 0;
	uint32 newTypeIndex = 0;
	for (uint32 oldTypeIndex = 0;
		oldTypeIndex < oldCount && newTypeIndex < newCount;
		++oldTypeIndex, ++newTypeIndex)
	{
		const ComponentType* oldType = oldOffsets.offsets[oldTypeIndex].type;
		Assert(oldType->size != 0);
		const ComponentType* newType = newOffsets.offsets[newTypeIndex].type;

		while (oldType != newType &&
			newTypeIndex < newCount)
		{
			++newTypeIndex;
			newType = newOffsets.offsets[newTypeIndex].type;
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

UniquePtr<ArchetypeChunk> CreateChunk(ComponentTypeOffsetList& offsetList)
{
	ArchetypeChunk* chunk = new ArchetypeChunk;
	chunk->footer.offsetList = &offsetList;

	return UniquePtr<ArchetypeChunk>(chunk);
}

void ConstructEntityInChunk(ArchetypeChunk& chunk, uint32 entityIndex)
{
	Assert(chunk.footer.numEntities > entityIndex);
	ComponentTypeOffsetList* offsetList = chunk.footer.offsetList;

	// Initialize memory using contrustor
	for (const auto& compOffset : offsetList->offsets)
	{
		const ComponentType compType = *compOffset.type;

		// TODO - Support empty classes because of tagging
		Assert(compType.size > 0);
		void* compAddr = GetComponentOffsetInChunk(chunk, compOffset) + GetComponentIndex(compType.size, entityIndex);
		compType.ctor(compAddr);
	}
}

void DestructEntityInChunk(ArchetypeChunk& chunk, uint32 entityIndex)
{
	Assert(chunk.footer.numEntities > entityIndex);
	ComponentTypeOffsetList* offsetList = chunk.footer.offsetList;

	// Initialize memory using contrustor
	for (const auto& compOffset : offsetList->offsets)
	{
		const ComponentType compType = *compOffset.type;

		// TODO - Support empty classes because of tagging
		Assert(compType.size > 0);
		void* compAddr = GetComponentOffsetInChunk(chunk, compOffset) + GetComponentIndex(compType.size, entityIndex);
		compType.dtor(compAddr);
	}
}

// Returns the new chunk index
uint32 MoveEntityToChunk(Entity& entity, ArchetypeChunk& oldChunk, uint32 oldChunkIndex, ArchetypeChunk& newChunk)
{
	uint32 oldComponentCount = oldChunk.footer.offsetList->capacity;
	uint32 newComponentCount = newChunk.footer.offsetList->capacity;
	auto& oldOffsetList = oldChunk.footer.offsetList;
	auto& newOffsetList = newChunk.footer.offsetList;

	uint32 newChunkIndex = AddEntityToChunk(newChunk, entity);


	//copy all data from old chunk into new chunk
	//bad iteration, fix later

	SameComponentType sameTypes[MaxComponentsPerArchetype];

	uint32 sameTypeCount = AssociateSameComponentTypes(
		sameTypes,
		*oldOffsetList, 
		*newOffsetList, 
		oldComponentCount, 
		newComponentCount);

	for (uint32 i = 0; i < sameTypeCount; ++i)
	{
		uint32 oldIndex = sameTypes[i].oldIndex;
		uint32 newIndex = sameTypes[i].newIndex;
		uint32 compSize = sameTypes[i].size;
		
		void* oldAddr = GetComponentOffsetInChunk(oldChunk, oldOffsetList->offsets[oldIndex]) + GetComponentIndex(compSize, oldChunkIndex);
		void* newAddr = GetComponentOffsetInChunk(newChunk, newOffsetList->offsets[newIndex]) + GetComponentIndex(compSize, newChunkIndex);

		Memcpy(newAddr, compSize, oldAddr, compSize);
	}

	return newChunkIndex;
}

uint32 AddEntityToChunk(ArchetypeChunk& chunk, const Entity& entity)
{
	REF_CHECK(chunk, entity);

	ComponentTypeOffsetList* offsetList = chunk.footer.offsetList;
	Assert(chunk.footer.numEntities < offsetList->capacity);

	uint32 entityIndex = chunk.footer.numEntities;
	++chunk.footer.numEntities;

	Entity* entityArray = reinterpret_cast<Entity*>(&chunk);
	new(entityArray + entityIndex) Entity{ entity };

	CheckIfFullChunk(chunk, offsetList->capacity);

	return entityIndex;
}
void RemoveEntityFromChunk(ArchetypeChunk& chunk, uint32 chunkIndex)
{
	Assert(chunk.footer.numEntities > chunkIndex);

	bool fullBeforeRemove = chunk.footer.numEntities == chunk.footer.offsetList->capacity;
	--chunk.footer.numEntities;

	DestructEntityInChunk(chunk, chunkIndex);

	Entity* entityArray = reinterpret_cast<Entity*>(&chunk);
	new(entityArray + chunkIndex) Entity{};

	CheckIfWasFullChunk(chunk, fullBeforeRemove);
}
}

