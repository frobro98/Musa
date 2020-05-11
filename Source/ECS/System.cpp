
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
DynamicArray<ArchetypeChunk> System::GetQueryChunks(const Query& query)
{
	// TODO - Maybe consider not allocating 
	uint32 chunkCount = 0;
	for (auto& archetype : query.queriedArchetypes)
	{
		chunkCount += archetype->chunks.Size();
	}

	DynamicArray<ArchetypeChunk> chunks;
	chunks.Reserve(chunkCount);
	for (auto& archetype : query.queriedArchetypes)
	{
		chunks.AddRange(archetype->chunks.GetData(), archetype->chunks.Size());
	}
	Assert(chunks.Capacity() == chunkCount);

	return chunks;
}
}