// Copyright 2020, Nathan Blane

#pragma once


#include "BasicTypes/Intrinsics.hpp"
#include "Utilities/MemoryUtilities.hpp"
#include "Containers/StaticArray.hpp"
#include "BasicTypes/UniquePtr.hpp"
#include "ECS/Types.hpp"
#include "ECS/ChunkArray.hpp"
#include "ECS/ComponentType.hpp"
#include "ECS/ECSAPI.hpp"

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
	u32* versions = nullptr;
	u32 componentTypeCount = 0;
	u32 entityCount = 0;
};

constexpr u32 UsableChunkSize = ArchetypeBlockSize - sizeof(ChunkHeader);

struct ArchetypeChunk
{
	ChunkHeader* header;
	u8* data;
};

static constexpr ArchetypeChunk EmptyChunk = { nullptr, nullptr };


void ConstructEntityInChunk(ArchetypeChunk& chunk, u32 entityIndex);
void DestructEntityInChunk(ArchetypeChunk& chunk, u32 entityIndex);

NODISCARD u32 AddEntityToChunk(ArchetypeChunk& chunk, const Entity& entity);
void RemoveEntityFromChunk(ArchetypeChunk& chunk, u32 chunkIndex);
void RemoveEntitiesFromChunk(ArchetypeChunk& chunk, DynamicArray<u32> chunkindices);
NODISCARD u32 MoveEntityToChunk(Entity& entity, ArchetypeChunk& oldChunk, u32 oldChunkIndex, ArchetypeChunk& newChunk);

template <typename Comp>
NODISCARD ECS_TEMPLATE forceinline bool DoesChunkContain(const ArchetypeChunk& chunk)
{
	static const ComponentType* lookupType = GetComponentTypeFor<Comp>();
	const ComponentType** types = chunk.header->types;
	u32 typeCount = chunk.header->componentTypeCount;
	for (u32 i = 0; i < typeCount; ++i)
	{
		if (lookupType == types[i])
		{
			return true;
		}
	}
	return false;
}

template <typename Comp>
NODISCARD ECS_TEMPLATE forceinline i32 GetChunkTypeIndex(const ArchetypeChunk& chunk)
{
	static const ComponentType* lookupType = GetComponentTypeFor<Comp>();

	i32 typeIndex = -1;
	const ComponentType** types = chunk.header->types;
	u32 typeCount = chunk.header->componentTypeCount;
	for (u32 i = 0; i < typeCount; ++i)
	{
		if (lookupType == types[i])
		{
			typeIndex = (i32)i;
			break;
		}
	}

	return typeIndex;
}

template <typename Comp>
NODISCARD ECS_TEMPLATE forceinline u32 GetChunkComponentVersion(const ArchetypeChunk& chunk)
{
	i32 versionIndex = GetChunkTypeIndex<Comp>(chunk);
	return versionIndex >= 0 ? chunk.header->versions[versionIndex] : 0;
}

template <typename Comp>
ECS_TEMPLATE forceinline void SetChunkComponentVersion(ArchetypeChunk& chunk, u32 newVersion)
{
	i32 versionIndex = GetChunkTypeIndex<Comp>(chunk);
	chunk.header->versions[versionIndex] = newVersion;
}

}

