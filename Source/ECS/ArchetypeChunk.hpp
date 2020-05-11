#pragma once


#include "Types/Intrinsics.hpp"
#include "EngineCore/MemoryUtilities.h"
#include "Containers/StaticArray.hpp"
#include "Types/UniquePtr.hpp"
#include "ECS/ChunkArray.hpp"
#include "ECS/ComponentType.hpp"


#pragma warning(push)
#pragma warning(disable:4307)
namespace Musa
{

struct Entity;
struct Archetype;

constexpr size_t ArchetypeBlockSize = KilobytesAsBytes(16);

struct ChunkHeader
{
	DynamicArray<const ComponentType*>* types = nullptr;
	DynamicArray<ComponentTypeHash>* typeHashes = nullptr;
	DynamicArray<size_t>* offsets = nullptr;
	Archetype* owner = nullptr;
	uint32 entityCount = 0;
};

constexpr uint32 UsableChunkSize = ArchetypeBlockSize - sizeof(ChunkHeader);

struct ArchetypeChunk
{
	ChunkHeader* header;
	uint8* data;
};

static constexpr ArchetypeChunk EmptyChunk = { nullptr, nullptr };


template<typename Comp>
NODISCARD forceinline ChunkArray<Comp> GetChunkArray(ArchetypeChunk& chunk)
{
	using sanitizedType = std::remove_reference_t<std::remove_const_t<Comp>>;

	if constexpr (std::is_same_v<Comp, Entity>)
	{
		Entity* entityArr = reinterpret_cast<Entity*>(chunk.data);
		return ChunkArray<Entity>(*entityArr, chunk.header->entityCount);
	}
	else
	{
		constexpr uint64 hash = Musa::Internal::TypenameHash<sanitizedType>();
		auto& hashes = *chunk.header->typeHashes;
		const uint32 typeCount = hashes.Size();

		if (typeCount > 0)
		{
			for (uint32 i = 0; i < typeCount; ++i)
			{
				if (hashes[i].typenameHash == hash)
				{
					auto& offsets = *chunk.header->offsets;
					sanitizedType* ptr = reinterpret_cast<sanitizedType*>(chunk.data + offsets[i]);
					const uint32 numEntities = chunk.header->entityCount;
					return ChunkArray<sanitizedType>(*ptr, numEntities);
				}
			}
		}

		return ChunkArray<sanitizedType>();
	}
}

void ConstructEntityInChunk(ArchetypeChunk& chunk, uint32 entityIndex);
void DestructEntityInChunk(ArchetypeChunk& chunk, uint32 entityIndex);

NODISCARD uint32 AddEntityToChunk(ArchetypeChunk& chunk, const Entity& entity);
void RemoveEntityFromChunk(ArchetypeChunk& chunk, uint32 chunkIndex);
NODISCARD uint32 MoveEntityToChunk(Entity& entity, ArchetypeChunk& oldChunk, uint32 oldChunkIndex, ArchetypeChunk& newChunk);
}
#pragma warning(pop)

