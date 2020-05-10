
#include "QueryCache.hpp"
#include "QueryDescription.hpp"
#include "ECS/ComponentType.hpp"
#include "ECS/World.hpp"

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
		ArchetypeWorksWithNoneTypes(archetype, qb.noneComp, qb.noneMask) &&
		ArchetypeWorksWithAnyTypes(archetype, qb.oneOrMoreComps, qb.oneOrMoreMask);
}

static bool DoesArchetypeMatchQuery(const Archetype& archetype, const Query& query)
{
	return ArchetypeContainsRequiredTypes(archetype, query.required, query.requiredMask) &&
		ArchetypeWorksWithNoneTypes(archetype, query.oneOrMore, query.oneOrMoreMask) &&
		ArchetypeWorksWithAnyTypes(archetype, query.none, query.noneMask);
}

static bool QueryEquals(const QueryDescription& desc, const Query& query)
{
	if (desc.requiredComps.Size() == query.required.Size() &&
		desc.noneComp.Size() == query.none.Size() &&
		desc.oneOrMoreComps.Size() == query.oneOrMore.Size())
	{
		for (uint32 i = 0; i < desc.requiredComps.Size(); ++i)
		{
			if (desc.requiredComps[i] != query.required[i])
			{
				return false;
			}
		}

		for (uint32 i = 0; i < desc.noneComp.Size(); ++i)
		{
			if (desc.noneComp[i] != query.none[i])
			{
				return false;
			}
		}

		for (uint32 i = 0; i < desc.oneOrMoreComps.Size(); ++i)
		{
			if (desc.oneOrMoreComps[i] != query.oneOrMore[i])
			{
				return false;
			}
		}

		return true;
	}
	

	return false;
}

static Query* CreateNewQuery(World& world, const QueryDescription& desc)
{
	Query* query = new Query;
	query->required = desc.requiredComps;
	query->oneOrMore = desc.oneOrMoreComps;
	query->none = desc.noneComp;
	query->requiredMask = desc.requiredMask;
	query->oneOrMoreMask = desc.oneOrMoreMask;
	query->noneMask = desc.noneMask;
	auto& archetypes = world.archetypes;
	for (const auto& archetype : archetypes)
	{
		if (DoesArchetypeMatchQueryTypes(*archetype, desc))
		{
			query->queriedArchetypes.Add(archetype.Get());
		}
	}
	return query;
}

//////////////////////////////////////////////////////////////////////////

QueryCache::QueryCache(World& w)
	: world(&w)
{
	REF_CHECK(w);
}

Query& QueryCache::GetOrCreateEntityQuery(const QueryDescription& desc)
{
	uint32 queryHash = 0;
	for (uint32 i = 0; i < desc.requiredComps.Size(); ++i)
	{
		HashCombine(queryHash, (uint32)desc.requiredComps[i]->hash.typenameHash);
	}

	for (uint32 i = 0; i < desc.oneOrMoreComps.Size(); ++i)
	{
		HashCombine(queryHash, (uint32)desc.oneOrMoreComps[i]->hash.typenameHash);
	}

	for (uint32 i = 0; i < desc.noneComp.Size(); ++i)
	{
		HashCombine(queryHash, (uint32)desc.noneComp[i]->hash.typenameHash);
	}

	HashCombine(queryHash, (uint32)((desc.requiredMask >> 32) & 0xFFFFFFFF));
	HashCombine(queryHash, (uint32)(desc.requiredMask & 0xFFFFFFFF));
	HashCombine(queryHash, (uint32)((desc.oneOrMoreMask >> 32) & 0xFFFFFFFF));
	HashCombine(queryHash, (uint32)(desc.oneOrMoreMask & 0xFFFFFFFF));
	HashCombine(queryHash, (uint32)((desc.noneMask >> 32) & 0xFFFFFFFF));
	HashCombine(queryHash, (uint32)(desc.noneMask & 0xFFFFFFFF));

	Query* query = nullptr;
	DynamicArray<Query*>* matchedQueries = matchingHashQueries.Find(queryHash);
	if (matchedQueries != nullptr)
	{
		for (auto* q : *matchedQueries)
		{
			if (QueryEquals(desc, *q))
			{
				query = q;
				break;
			}
		}
		// Query not found
		if (query == nullptr)
		{
			query = CreateNewQuery(*world, desc);
			matchedQueries->Add(query);
		}
	}
	else
	{
		query = CreateNewQuery(*world, desc);
		DynamicArray<Query*> arr = { query };
		matchingHashQueries.Add(queryHash, std::move(arr));
	}

	return *query;
}

void QueryCache::AddNewArchetypeToQueries(Archetype** archetypes, uint32 archetypeCount)
{
	for (auto* query : queries)
	{
		for (uint32 i = 0; i < archetypeCount; ++i)
		{
			Archetype& archetype = *archetypes[i];
			if (DoesArchetypeMatchQuery(archetype, *query))
			{
				query->queriedArchetypes.Add(&archetype);
			}
		}
	}
}
}