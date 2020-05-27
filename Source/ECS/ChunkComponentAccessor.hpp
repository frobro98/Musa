// Copyright 2020, Nathan Blane

#pragma once

#include "Containers/ArrayView.hpp"
#include "ECS/ArchetypeChunk.hpp"
#include "ECS/Types.hpp"
#include "ECS/ECSAPI.hpp"

namespace Musa
{
struct ECS_API ChunkComponentAccessor
{
	ChunkComponentAccessor(ArchetypeChunk& chunk);

	NODISCARD ArrayView<const ComponentType*> GetChunkTypes() const;
	NODISCARD forceinline u32 GetEntityCount() const
	{
		return chunk.header->entityCount;
	}

	template <typename CompType, typename = std::enable_if_t<is_valid_component_type_v<CompType>>>
	NODISCARD bool Contains() const;
	template <typename CompType, typename = std::enable_if_t<is_valid_component_type_v<CompType>>>
	NODISCARD bool HasChanged(u32 checkVersion) const;

	template <typename CompType, typename = std::enable_if_t<is_valid_component_type_v<CompType>>>
	NODISCARD ChunkArray<CompType> GetArrayOf();


	ArchetypeChunk chunk;
};
template<typename CompType, typename>
inline bool ChunkComponentAccessor::Contains() const
{
	return DoesChunkContain<CompType>(chunk);
}
template<typename CompType, typename>
inline bool ChunkComponentAccessor::HasChanged(u32 checkVersion) const
{
	// TODO - Actually check if this is more likely or not. In theory it should be more likely because it's one number out of uint_max - 1
	if (likely(checkVersion != 0))
	{
		u32 version = GetChunkComponentVersion<CompType>(chunk);
		return (i32)(version - checkVersion) > 0;
	}
	return true;
}
template<typename CompType, typename>
inline ChunkArray<CompType> ChunkComponentAccessor::GetArrayOf()
{
	return GetChunkArray<CompType>(chunk);
}
}
