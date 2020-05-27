// Copyright 2020, Nathan Blane

#pragma once

#include "Containers/DynamicArray.hpp"
#include "Containers/ArrayView.hpp"
#include "BasicTypes/Uncopyable.hpp"
#include "ECS/Types.hpp"
#include "ECS/ECSAPI.hpp"
#include "ECS/ComponentType.hpp"
#include "ECS/Entity.hpp"


namespace Musa
{

struct Archetype;

// Things a Query object should be able to do
//  - Get all entities associated with this query (Done)
//  - Get all chunks within the relevant Archetypes
//  - Get all required ComponentTypes in the query (Done)
//  - 

// Things a Query object would be involved in performing
//  - 
//  - 

// TODO - Might want to query information about what this query represents
// Used to get all of the entities that match a specific query
struct Query : Uncopyable
{
	// TODO - Need a way to update this list when new archetypes are added
	DynamicArray<Archetype*> queriedArchetypes;

	// Component Types that are going to be used in this query
	// TODO - What is the difference between this data and the query description?
	// Could it be usage? Cache related?
	DynamicArray<const ComponentType*> required;
	DynamicArray<const ComponentType*> oneOrMore;
	DynamicArray<const ComponentType*> none;
	ArchetypeMask requiredMask;
	ArchetypeMask oneOrMoreMask;
	ArchetypeMask noneMask;
};

ArrayView<Entity> ECS_API GetQueriedEntities(const Query& query);


void ResetInternalCache();

}
