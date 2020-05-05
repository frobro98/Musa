#pragma once

#include "Containers/DynamicArray.hpp"
#include "ECS/ComponentType.hpp"


namespace Musa
{

struct Archetype;

// TODO - Might want to query information about what this query represents
// Used to get all of the entities that match a specific query
struct Query
{
	// TODO - Need a way to update this list when new archetypes are added
	DynamicArray<Archetype*> queriedArchetypes;

	// Component Types that are going to be used in this query
};

}
