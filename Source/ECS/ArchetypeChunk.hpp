#pragma once

#include "Types/Intrinsics.hpp"
#include "Types/UniquePtr.hpp"

namespace Musa
{

struct Entity;
struct Archetype;
struct ComponentTypeOffsetList;

constexpr size_t ArchetypeBlockSize = KilobytesAsBytes(16);

struct ArchetypeDataFooter
{
	ComponentTypeOffsetList* offsetList = nullptr;
	Archetype* owner = nullptr;
	uint32 numEntities = 0;
};

constexpr uint32 UsableChunkSize = ArchetypeBlockSize - sizeof(ArchetypeDataFooter);

struct /*alignas(32)*/ ArchetypeChunk
{
	uint8 data[UsableChunkSize];
	ArchetypeDataFooter footer;
};
static_assert(sizeof(ArchetypeChunk) == ArchetypeBlockSize);

UniquePtr<ArchetypeChunk> CreateChunk(ComponentTypeOffsetList& offsetList);

void ConstructEntityInChunk(ArchetypeChunk& chunk, uint32 entityIndex);
void DestructEntityInChunk(ArchetypeChunk& chunk, uint32 entityIndex);

[[nodiscard]] uint32 AddEntityToChunk(ArchetypeChunk& chunk, const Entity& entity);
void RemoveEntityFromChunk(ArchetypeChunk& chunk, uint32 chunkIndex);
[[nodiscard]] uint32 MoveEntityToChunk(Entity& entity, ArchetypeChunk& oldChunk, uint32 oldChunkIndex, ArchetypeChunk& newChunk);
}

