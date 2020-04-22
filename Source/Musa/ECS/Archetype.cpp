
#include "Archetype.hpp"
#include "Entity.hpp"
#include "ComponentType.hpp"
#include "World.hpp"

namespace Musa
{
static constexpr ArchetypeHashID BuildArchetypeHash(const ComponentType** types, size_t typeCount)
{
	ArchetypeHashID hashId = 0;
	for (size_t i = 0; i < typeCount; ++i)
	{
		hashId |= types[i]->archetypeBit;
	}
	return hashId;
}

static forceinline ArchetypeChunk* CreateNewChunkFor(Archetype& archetype)
{
	UniquePtr<ArchetypeChunk> chunk = CreateChunk(archetype.offsetList);
	chunk->footer.owner = &archetype;
	archetype.chunkList.Add(std::move(chunk));
	return chunk.Get();
}

static Archetype* FindMatchingArchetype(World& world, ArchetypeHashID hashId, const ComponentType** compTypes, size_t typeCount)
{
	Archetype* archetype = nullptr;
	auto iter = world.archetypesByHash.find(hashId);
	if (iter != world.archetypesByHash.end())
	{
		auto& archetypeList = iter->second;
		for (auto& archetypeElem : archetypeList)
		{
			auto& offsetList = archetypeElem->offsetList;
			size_t compCount = offsetList.offsets.Size();
			if (compCount == typeCount)
			{
				bool found = true;
				for (uint32 j = 0; j < compCount; ++j)
				{
					if (offsetList.offsets[j].type != compTypes[j])
					{
						found = false;
						break;
					}
				}

				if (found)
				{
					archetype = archetypeElem;
					break;
				}
			}
		}
	}
	return archetype;
}

static void AssociateNewEntityTo(Archetype& archetype, Entity entity)
{
	// New entity means finding a free data chunk and putting itself in it
	ArchetypeChunk& chunk = GetOrCreateFreeArchetypeChunk(archetype);
	uint32 chunkIndex = AddEntityToChunk(chunk, entity);
	ConstructEntityInChunk(chunk, chunkIndex);
	archetype.world->entityBridges[entity.id].owningChunk = &chunk;
	archetype.world->entityBridges[entity.id].chunkIndex = chunkIndex;
}

static void MoveEntityTo(Archetype& archetype, Entity entity)
{
	World& world = *archetype.world;
	ArchetypeChunk& oldChunk = *world.entityBridges[entity.id].owningChunk;
	REF_CHECK(oldChunk);

	uint32 oldChunkIndex = world.entityBridges[entity.id].chunkIndex;

	ArchetypeChunk& newChunk = GetOrCreateFreeArchetypeChunk(archetype);

	uint32 newChunkIndex = MoveEntityToChunk(entity, oldChunk, oldChunkIndex, newChunk);

	RemoveEntityFromChunk(oldChunk, oldChunkIndex);

	world.entityBridges[entity.id].owningChunk = &newChunk;
	world.entityBridges[entity.id].chunkIndex = newChunkIndex;
}

Archetype* GetOrCreateArchetypeFrom(World& world, const ComponentType** compTypes, size_t typeCount)
{
	REF_CHECK(world);
	Assert(IsSorted(compTypes, typeCount, LessThan<const ComponentType*>{}));

	Assert(typeCount <= MaxComponentsPerArchetype);

	ArchetypeHashID hashId = BuildArchetypeHash(compTypes, typeCount);

	Archetype* archetype = FindMatchingArchetype(world, hashId, compTypes, typeCount);
	if (archetype == nullptr)
	{
		UniquePtr<Archetype> upArchetype(new Archetype
			{
				&world,
				DynamicArray<ArchetypeChunk*>(),
				ConstructOffsetList(compTypes, typeCount),
				hashId,
				0
			});
		world.archetypeHashIDs.Add(upArchetype->archetypeHashID);
		world.archetypesByHash[hashId].Add(upArchetype.Get());
		world.archetypes.Add(std::move(upArchetype));
		archetype = upArchetype.Get();
	}

	return archetype;
}

ArchetypeChunk& GetOrCreateFreeArchetypeChunk(Archetype& archetype)
{
	ArchetypeChunk* chunk;
	if (archetype.chunkList.IsEmpty())
	{
		chunk = CreateNewChunkFor(archetype);
	}
	else
	{
		chunk = archetype.chunkList.Last().Get();
		if (chunk->footer.numEntities == archetype.offsetList.capacity)
		{
			chunk = CreateNewChunkFor(archetype);
		}
	}
	return *chunk;
}

void SetEntitysArchetype(World& world, Entity entity, Archetype& archetype)
{
	if (world.entityBridges[entity.id].owningChunk == nullptr)
	{
		AssociateNewEntityTo(archetype, entity);
	}
	else
	{
		MoveEntityTo(archetype, entity);
	}
}

void SortChunksForFullness(Archetype& archetype)
{
	// TODO - This implementation is essentially std::partition. Consider the use of turning this into a general impl
	// TODO - Could support find if and find if not algos

	const uint32 maxSize = archetype.offsetList.capacity;
	auto& chunks = archetype.chunkList;
	auto isChunkFull = [maxSize](ArchetypeChunk& chunk) {
		return chunk.footer.numEntities == maxSize;
	};

	// Find if not
	uint32 first;
	for (first = 0; first < chunks.Size(); ++first)
	{
		if (!isChunkFull(*chunks[first]))
		{
			break;
		}
	}

	for (uint32 index = first + 1; index < chunks.Size(); ++index)
	{
		if (isChunkFull(*chunks[index]))
		{
			Swap(chunks[index], chunks[first]);
			++first;
		}
	}
}
}
