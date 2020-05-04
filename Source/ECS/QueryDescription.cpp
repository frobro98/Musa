#include "QueryDescription.hpp"
#include "Query.hpp"
#include "World.hpp"
#include "Archetype.hpp"
#include "System.hpp"

namespace Musa
{
Query QueryDescription::ToQuery() const
{
	Assert(systemQuery);
	// TODO - Queries should be cached somewhere in the ECS ecosystem
	return systemQuery->GetQueryFor(*this);
}

// Query QueryDescription::ToQuery() const
// {
// 	Query q;
// 	auto& archetypes = world.archetypes;
// 	for (const auto& archetype : archetypes)
// 	{
// 		if (DoesArchetypeMatchQueryTypes(*archetype, *this))
// 		{
// 			q.queriedArchetypes.Add(archetype.Get());
// 		}
// 	}
// 	// TODO - Queries should be cached somewhere in the ECS ecosystem
// 	return q;
// }
}
