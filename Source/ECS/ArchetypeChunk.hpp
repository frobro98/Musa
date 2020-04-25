#pragma once

#include "Types/Intrinsics.hpp"
#include "Containers/StaticArray.hpp"
#include "Types/UniquePtr.hpp"
#include "ECS/ChunkArray.hpp"

#pragma warning(push)
#pragma warning(disable:4307)
namespace Musa
{

struct Entity;
struct Archetype;

constexpr size_t ArchetypeBlockSize = KilobytesAsBytes(16);

struct ArchetypeDataFooter
{
	DynamicArray<const ComponentType*>* types = nullptr;
	DynamicArray<ComponentTypeHash>* typeHashes = nullptr;
	DynamicArray<size_t>* offsets = nullptr;
	Archetype* owner = nullptr;
	uint32 numEntities = 0;
};

constexpr uint32 UsableChunkSize = ArchetypeBlockSize - sizeof(ArchetypeDataFooter);

struct ArchetypeChunk
{
	// TODO - Consider making this a dynamic block of memory instead of inline to the struct. 
	// This will speed up access to the footer/any other data in the chunk
	StaticArray<uint8, UsableChunkSize> data;
	ArchetypeDataFooter footer;
};
static_assert(sizeof(ArchetypeChunk) == ArchetypeBlockSize);

template<typename Comp>
inline ChunkArray<Comp> GetChunkArray(ArchetypeChunk& chunk)
{
	using sanitizedType = std::remove_reference_t<std::remove_const_t<Comp>>;

	if (chunk.footer.numEntities > 0)
	{
		if constexpr (std::is_same_v<Comp, Entity>)
		{
			Entity* entityArr = reinterpret_cast<Entity*>(&chunk);
			return ChunkArray<Entity>(*entityArr, chunk.footer.numEntities);
		}
		else
		{
			constexpr uint64 hash = Musa::Internal::TypenameHash<sanitizedType>();
			auto& hashes = *chunk.footer.typeHashes;
			const uint32 typeCount = hashes.Size();

			if (typeCount > 0)
			{
				for (uint32 i = 0; i < typeCount; ++i)
				{
					if (hashes[i].typenameHash == hash)
					{
						auto& offsets = *chunk.footer.offsets;
						sanitizedType* ptr = reinterpret_cast<sanitizedType*>((uint8*)&chunk + offsets[i]);
						const uint32 numEntities = chunk.footer.numEntities;
						return ChunkArray<sanitizedType>(*ptr, numEntities);
					}
				}
			}
		}
	}

	return ChunkArray<sanitizedType>();
}

void ConstructEntityInChunk(ArchetypeChunk& chunk, uint32 entityIndex);
void DestructEntityInChunk(ArchetypeChunk& chunk, uint32 entityIndex);

[[nodiscard]] uint32 AddEntityToChunk(ArchetypeChunk& chunk, const Entity& entity);
void RemoveEntityFromChunk(ArchetypeChunk& chunk, uint32 chunkIndex);
[[nodiscard]] uint32 MoveEntityToChunk(Entity& entity, ArchetypeChunk& oldChunk, uint32 oldChunkIndex, ArchetypeChunk& newChunk);
}
#pragma warning(pop)

