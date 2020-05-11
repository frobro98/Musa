
#include "Containers/FixedArray.hpp"
#include "Query.hpp"
#include "Archetype.hpp"

namespace Musa
{
constexpr size_t StaticEntityQuerySize = KilobytesAsBytes(8) / sizeof(Entity);
static FixedArray<Entity, (uint32)StaticEntityQuerySize> queriedEntityCache;

static void StoreQueriedArchetypeEntities(Archetype& archetype)
{
	for (auto& chunk : archetype.chunks)
	{
		ChunkArray<Entity> entities = GetChunkArray<Entity>(chunk);
		Assert(entities.IsValid());

		queriedEntityCache.AddRange(entities.data, entities.size);
	}
}

ArrayView<Entity> GetQueriedEntities(const Query& query)
{
	for (auto& archetype : query.queriedArchetypes)
	{
		StoreQueriedArchetypeEntities(*archetype);
	}
	return ArrayView<Entity>(queriedEntityCache.GetData(), queriedEntityCache.Size());
}

void ResetInternalCache()
{
	queriedEntityCache.Clear();
}
}