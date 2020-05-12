// Copyright 2020, Nathan Blane

#pragma once


#include "Types/Intrinsics.hpp"
#include "EngineCore/MemoryUtilities.h"
#include "Containers/StaticArray.hpp"
#include "Types/UniquePtr.hpp"
#include "ECS/ChunkArray.hpp"
#include "ECS/ComponentType.hpp"
#include "ECS/DLLDef.h"


namespace Musa
{

struct Entity;
struct Archetype;

constexpr size_t ArchetypeBlockSize = KilobytesAsBytes(16);

struct ChunkHeader
{
	Archetype* archetype = nullptr;
	const ComponentType** types = nullptr;
	ComponentTypeHash* typeHashes = nullptr;
	size_t* offsets = nullptr;
	uint32* versions = nullptr;
	uint32 componentTypeCount = 0;
	uint32 entityCount = 0;
};

constexpr uint32 UsableChunkSize = ArchetypeBlockSize - sizeof(ChunkHeader);

struct ArchetypeChunk
{
	ChunkHeader* header;
	uint8* data;
};

static constexpr ArchetypeChunk EmptyChunk = { nullptr, nullptr };


void ConstructEntityInChunk(ArchetypeChunk& chunk, uint32 entityIndex);
void DestructEntityInChunk(ArchetypeChunk& chunk, uint32 entityIndex);

NODISCARD uint32 AddEntityToChunk(ArchetypeChunk& chunk, const Entity& entity);
void RemoveEntityFromChunk(ArchetypeChunk& chunk, uint32 chunkIndex);
NODISCARD uint32 MoveEntityToChunk(Entity& entity, ArchetypeChunk& oldChunk, uint32 oldChunkIndex, ArchetypeChunk& newChunk);

// TODO - Consider not having this function exist, now that there's the ChunkComponentAccessor. The one issue would be World::SetComponentDataOn
template<typename Comp>
NODISCARD ECS_TEMPLATE forceinline ChunkArray<Comp> GetChunkArray(ArchetypeChunk& chunk)
{
	if constexpr (std::is_same_v<Comp, Entity>)
	{
		Entity* entityArr = reinterpret_cast<Entity*>(chunk.data);
		return ChunkArray<Entity>(*entityArr, chunk.header->entityCount);
	}
	else
	{
		using sanitizedType = std::remove_reference_t<std::remove_const_t<Comp>>;
		using nonRefType = std::remove_reference_t<Comp>;

		constexpr bool isConst = std::is_const_v<Comp>;
		constexpr uint64 hash = Musa::Internal::TypenameHash<sanitizedType>();
		ComponentTypeHash* hashes = chunk.header->typeHashes;
		const uint32 typeCount = chunk.header->componentTypeCount;

		if (typeCount > 0)
		{
			for (uint32 i = 0; i < typeCount; ++i)
			{
				if (hashes[i].typenameHash == hash)
				{
					if constexpr (!isConst)
					{
						// TODO - This is awful and needs to change. Might need to pass in the system or world or something, but it needs to change
						chunk.header->versions[i] = chunk.header->archetype->world->GetSystemVersion();
					}

					size_t* offsets = chunk.header->offsets;
					nonRefType* ptr = reinterpret_cast<nonRefType*>(chunk.data + offsets[i]);
					const uint32 numEntities = chunk.header->entityCount;
					return ChunkArray<nonRefType>(*ptr, numEntities);
				}
			}
		}

		return ChunkArray<nonRefType>();
	}
}

template <typename Comp>
NODISCARD ECS_TEMPLATE forceinline bool DoesChunkContain(const ArchetypeChunk& chunk)
{
	static const ComponentType* lookupType = GetComponentTypeFor<Comp>();
	const ComponentType** types = chunk.header->types;
	uint32 typeCount = chunk.header->componentTypeCount;
	for (uint32 i = 0; i < typeCount; ++i)
	{
		if (lookupType == types[i])
		{
			return true;
		}
	}
	return false;
}

template <typename Comp>
NODISCARD ECS_TEMPLATE forceinline int32 GetChunkTypeIndex(const ArchetypeChunk& chunk)
{
	static const ComponentType* lookupType = GetComponentTypeFor<Comp>();

	int32 typeIndex = -1;
	const ComponentType** types = chunk.header->types;
	uint32 typeCount = chunk.header->componentTypeCount;
	for (uint32 i = 0; i < typeCount; ++i)
	{
		if (lookupType == types[i])
		{
			typeIndex = (int32)i;
			break;
		}
	}

	return typeIndex;
}

template <typename Comp>
NODISCARD ECS_TEMPLATE forceinline uint32 GetChunkComponentVersion(const ArchetypeChunk& chunk)
{
	int32 versionIndex = GetChunkTypeIndex<Comp>(chunk);
	return chunk.header->versions[versionIndex];
}

template <typename Comp>
ECS_TEMPLATE forceinline void SetChunkComponentVersion(ArchetypeChunk& chunk, uint32 newVersion)
{
	int32 versionIndex = GetChunkTypeIndex<Comp>(chunk);
	chunk.header->versions[versionIndex] = newVersion;
}

}

