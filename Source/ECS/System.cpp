
#include "System.hpp"
#include "Query.hpp"
#include "World.hpp"
#include "QueryDescription.hpp"
#include "Archetype.hpp"

namespace Musa
{
static bool ArchetypeContainsRequiredTypes(const Archetype& archetype, const DynamicArray<const ComponentType*>& requiredTypes, ArchetypeMask requiredMask)
{
	ArchetypeMask reqMask = archetype.archetypeMask & requiredMask;

	if (reqMask != requiredMask)
	{
		return false;
	}

	const uint32 reqCount = requiredTypes.Size();
	uint32 matchedCount = 0;
	for (const auto& reqType : requiredTypes)
	{
		for (const auto& type : archetype.types)
		{
			if (reqType == type)
			{
				++matchedCount;
				break;
			}
		}
	}

	return reqCount == matchedCount;
}

static bool ArchetypeWorksWithAnyTypes(const Archetype& archetype, const DynamicArray<const ComponentType*>& anyTypes, ArchetypeMask anyMask)
{
	if (anyTypes.IsEmpty())
	{
		return true;
	}

	ArchetypeMask mask = archetype.archetypeMask & anyMask;
	if (mask != 0)
	{
		for (const auto& anyType : anyTypes)
		{
			for (const auto& type : archetype.types)
			{
				if (anyType == type)
				{
					return true;
				}
			}
		}
	}

	return false;
}

static bool ArchetypeWorksWithNoneTypes(const Archetype& archetype, const DynamicArray<const ComponentType*>& noneTypes, ArchetypeMask noneMask)
{

	ArchetypeMask mask = archetype.archetypeMask & noneMask;
	if (mask == 0)
	{
		return true;
	}

	for (const auto& noneType : noneTypes)
	{
		for (const auto& type : archetype.types)
		{
			if (noneType == type)
			{
				return false;
			}
		}
	}

	return true;

}

static bool DoesArchetypeMatchQueryTypes(const Archetype& archetype, const QueryDescription& qb)
{
	return ArchetypeContainsRequiredTypes(archetype, qb.requiredComps, qb.requiredMask) &&
		ArchetypeWorksWithNoneTypes(archetype, qb.oneOrMoreComps, qb.anyMask) &&
		ArchetypeWorksWithAnyTypes(archetype, qb.noneComp, qb.noneMask);
}

System::System(World& w)
	: world(&w)
{
}

Query System::GetQueryFor(const QueryDescription& qb)
{
	// TODO - To prevent this from happening a bunch, this needs to be cached either in the system or in the world/whatever would hold 
	// a cache of queries
	Query q;
	auto& archetypes = world->archetypes;
	for (const auto& archetype : archetypes)
	{
		if (DoesArchetypeMatchQueryTypes(*archetype, qb))
		{
			q.queriedArchetypes.Add(archetype.Get());
		}
	}
	return q;
}

// QueryDescription System::BeginQuery()
// {
// 	return QueryDescription();
// }
//
// Query System::EndQuery(const QueryDescription& qb)
// {
// 	return GetQueryUsing(qb);
// }
}