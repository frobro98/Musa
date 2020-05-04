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
	DynamicArray<Archetype*> queriedArchetypes;
};

}
