
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
}