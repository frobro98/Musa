// Copyright 2020, Nathan Blane

#include "System.hpp"
#include "Query.hpp"
#include "World.hpp"
#include "QueryDescription.hpp"
#include "Archetype.hpp"

namespace Musa
{
void System::InitializeInternals(World& w)
{
	world = &w;
	queryCache = world->queryCache;
}

Query& System::GetQueryFor(const QueryDescription& qb)
{
	return queryCache->GetOrCreateEntityQuery(qb);
}
DynamicArray<ChunkComponentAccessor> System::GetQueryChunks(const Query& query)
{
	// TODO - Maybe consider not allocating 
	u32 chunkCount = 0;
	for (auto& archetype : query.queriedArchetypes)
	{
		chunkCount += archetype->chunks.Size();
	}

	DynamicArray<ChunkComponentAccessor> chunks;
	chunks.Reserve(chunkCount);
	for (auto& archetype : query.queriedArchetypes)
	{
		for (u32 i = 0; i < archetype->chunks.Size(); ++i)
		{
			chunks.Add(ChunkComponentAccessor(archetype->chunks[i])); 
		}
	}
	Assert(chunks.Capacity() == chunkCount);

	return chunks;
}
}