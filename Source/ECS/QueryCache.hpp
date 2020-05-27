// Copyright 2020, Nathan Blane

#pragma once

#include "Containers/Map.h"
#include "ECS/Query.hpp"

namespace Musa
{
class World;
struct QueryDescription;

class QueryCache
{
public:
	QueryCache(World& w);
	// Methods to get or create queries depending on specific input (ComponentTypes and some sort of hash of them)
	Query& GetOrCreateEntityQuery(const QueryDescription& desc);

	// Methods to update queries if there are new archetypes in the system
	void AddNewArchetypeToQueries(Archetype** archetype, u32 archetypeCount);

private:
	// Cache of queries
	DynamicArray<Query*> queries;
	Map<u32, DynamicArray<Query*>> matchingHashQueries;
	World* world = nullptr;
};
}