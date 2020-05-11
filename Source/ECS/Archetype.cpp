
#include "Archetype.hpp"
#include "Entity.hpp"
#include "ComponentType.hpp"
#include "World.hpp"

namespace Musa
{
static constexpr ArchetypeMask BuildArchetypeHash(const ComponentType** types, size_t typeCount)
{
	ArchetypeMask hashId = 0;
	for (size_t i = 0; i < typeCount; ++i)
	{
		hashId |= types[i]->hash.archetypeBit;
	}
	return hashId;
}

static void FillOutTypeInformation(Archetype& archetype, const ComponentType** compTypes, size_t count)
{
	uint32 componentSetSize = sizeof(Entity);
	for (size_t i = 0; i < count; ++i)
	{
		componentSetSize += compTypes[i]->size;
	}

	constexpr uint32 usableSpace = Musa::UsableChunkSize;
	// slack just pads chunk for safety.
	// TODO - Investigate if this is actually necessary...
	constexpr uint32 slack = 2;
	const uint32 archetypeEntityCap = usableSpace / componentSetSize - slack;
	archetype.entityCapacity = archetypeEntityCap;

	auto& typeList = archetype.types;
	auto& typeHashList = archetype.typeHashes;
	auto& typeOffsetList = archetype.offsets;
	typeList.Reserve((uint32)count);
	typeHashList.Reserve((uint32)count);
	typeOffsetList.Reserve((uint32)count);


	size_t offset = sizeof(Entity) * archetypeEntityCap;
	for (size_t i = 0; i < count; ++i)
	{
		const ComponentType* type = compTypes[i];
		Assert(type->alignment != 0);

		size_t remainder = offset % type->alignment;
		offset += type->alignment - remainder;
		
		typeList.Add(type);
		typeHashList.Add(type->hash);
		typeOffsetList.Add(offset);

		offset += type->size * archetypeEntityCap;
	}

	Assert(offset <= Musa::ArchetypeBlockSize);
}

static forceinline ArchetypeChunk CreateNewChunkFor(Archetype& archetype)
{
	uint8* chunkBlock = new uint8[ArchetypeBlockSize];
	ChunkHeader* header = new(chunkBlock) ChunkHeader;
	uint8* chunkData = reinterpret_cast<uint8*>(header + 1);
	ArchetypeChunk chunk;
	chunk.header = header;
	chunk.data = chunkData;

	header->owner = &archetype;
	header->types = &archetype.types;
	header->typeHashes = &archetype.typeHashes;
	header->offsets = &archetype.offsets;
	archetype.chunks.Add(chunk);
	return chunk;
}

// TODO - There needs to be a way to destroy the memory holding onto the chunk data...

static Archetype* FindMatchingArchetype(World& world, ArchetypeMask hashId, const ComponentType** compTypes, size_t typeCount)
{
	Archetype* archetype = nullptr;
	auto iter = world.archetypesByHash.find(hashId);
	if (iter != world.archetypesByHash.end())
	{
		auto& archetypeList = iter->second;
		for (auto& archetypeElem : archetypeList)
		{
			auto& typeList = archetypeElem->types;
			size_t compCount = typeList.Size();
			if (compCount == typeCount)
			{
				bool found = true;
				for (uint32 j = 0; j < compCount; ++j)
				{
					if (typeList[j] != compTypes[j])
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
	ArchetypeChunk chunk = GetOrCreateFreeArchetypeChunk(archetype);
	uint32 chunkIndex = AddEntityToChunk(chunk, entity);
	ConstructEntityInChunk(chunk, chunkIndex);
	archetype.world->entityBridges[entity.id].chunk = chunk;
	archetype.world->entityBridges[entity.id].chunkIndex = chunkIndex;
}

static void MoveEntityTo(Archetype& archetype, Entity entity)
{
	World& world = *archetype.world;
	ArchetypeChunk oldChunk = world.entityBridges[entity.id].chunk;
	REF_CHECK(oldChunk);

	uint32 oldChunkIndex = world.entityBridges[entity.id].chunkIndex;

	ArchetypeChunk newChunk = GetOrCreateFreeArchetypeChunk(archetype);

	uint32 newChunkIndex = MoveEntityToChunk(entity, oldChunk, oldChunkIndex, newChunk);

	RemoveEntityFromChunk(oldChunk, oldChunkIndex);

	world.entityBridges[entity.id].chunk = newChunk;
	world.entityBridges[entity.id].chunkIndex = newChunkIndex;
}

Archetype* GetOrCreateArchetypeFrom(World& world, const ComponentType** compTypes, size_t typeCount)
{
	REF_CHECK(world);
	Assert(IsSorted(compTypes, typeCount, Less<const ComponentType*>{}));
	Assert(typeCount <= MaxComponentsPerArchetype);

	Internal::CheckForSameComponents(compTypes, typeCount);

	ArchetypeMask hashId = BuildArchetypeHash(compTypes, typeCount);

	Archetype* archetype = FindMatchingArchetype(world, hashId, compTypes, typeCount);
	if (archetype == nullptr)
	{
		archetype = new Archetype;
		archetype->world = &world;
		archetype->archetypeMask = hashId;
		archetype->totalEntityCount = 0;
		archetype->fullChunkCount = 0;

		FillOutTypeInformation(*archetype, compTypes, typeCount);
		UniquePtr<Archetype> upArchetype(archetype);

		world.archetypeHashIDs.Add(upArchetype->archetypeMask);
		world.archetypesByHash[hashId].Add(upArchetype.Get());
		world.archetypes.Add(std::move(upArchetype));
	}

	return archetype;
}

ArchetypeChunk GetOrCreateFreeArchetypeChunk(Archetype& archetype)
{
	ArchetypeChunk chunk;
	if (archetype.chunks.IsEmpty())
	{
		chunk = CreateNewChunkFor(archetype);
	}
	else
	{
		chunk = archetype.chunks.Last();
		if (chunk.header->entityCount == archetype.entityCapacity)
		{
			chunk = CreateNewChunkFor(archetype);
		}
	}
	return chunk;
}

void SetEntitysArchetype(World& world, Entity entity, Archetype& archetype)
{
	if (Memcmp(&world.entityBridges[entity.id].chunk, &EmptyChunk, sizeof(ArchetypeChunk)) == 0)
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

	const uint32 maxSize = archetype.entityCapacity;
	auto& chunks = archetype.chunks;
	auto isChunkFull = [maxSize](ArchetypeChunk& chunk) {
		return chunk.header->entityCount == maxSize;
	};

	// Find if not
	uint32 first;
	for (first = 0; first < chunks.Size(); ++first)
	{
		if (!isChunkFull(chunks[first]))
		{
			break;
		}
	}

	for (uint32 index = first + 1; index < chunks.Size(); ++index)
	{
		if (isChunkFull(chunks[index]))
		{
			Swap(chunks[index], chunks[first]);
			++first;
		}
	}
}
}
