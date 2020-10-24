// Copyright 2020, Nathan Blane

#pragma once

#include "Containers/DynamicArray.hpp"
#include "Containers/Map.h"
#include "Algorithms/Algorithms.hpp"
#include "Utilities/Array.hpp"
#include "ECS/ComponentType.hpp"
#include "ECS/Types.hpp"

namespace Musa
{
struct Query;
struct ComponentType;
class System;

forceinline ArchetypeMask BuildQueryMask(const ComponentType** types, size_t typeCount)
{
	ArchetypeMask mask = 0;
	for (size_t i = 0; i < typeCount; ++i)
	{
		mask |= types[i]->hash.archetypeBit;
	}
	return mask;
}

struct QueryDescription
{
	// Require can take up to MaxComponentsPerArchetype Components. 
	// Components don't need to be part of the ForEach processing to be required
	template <typename... Comps>
	QueryDescription& Require();
	// Any can take up to MaxComponentsPerArchetype Components. 
	// Components don't need to be part of the ForEach processing to be Any
	template <typename... Comps>
	QueryDescription& OneOrMore();
	// None can take up to MaxComponentsPerArchetype Components. 
	// Components don't need to be part of the ForEach processing to be none
	template <typename... Comps>
	QueryDescription& None();

	Query& ToQuery() const;

	DynamicArray<const ComponentType*> requiredComps;
	DynamicArray<const ComponentType*> oneOrMoreComps;
	DynamicArray<const ComponentType*> noneComp;
	ArchetypeMask requiredMask = 0;
	ArchetypeMask oneOrMoreMask = 0;
	ArchetypeMask noneMask = 0;
	// TODO - It doesn't really make much sense to keep the world around I feel. The world will eventually manage the systems
	// that exist as well as entity stuff, but it's way too big of a class to keep around at this level
	//World* world;
	System* systemQuery;
};

template<typename... Comps>
inline QueryDescription& QueryDescription::Require()
{
	constexpr u32 CompCount = sizeof...(Comps);
	static_assert(CompCount < MaxComponentsPerArchetype,
		"No Archetype has this many components. Check MaxComponentsPerArchetype for details");

	static const ComponentType* types[] = { GetComponentTypeFor<Comps>()... };
	static_assert(ArraySize(types) == CompCount, "Number of ComponentType ptrs is wrong");

	InsertionSort(types, CompCount);

	// TODO - validate that there aren't duplicates.

	Assert(requiredComps.IsEmpty());
	requiredComps.Reserve(CompCount);
	requiredComps.AddRange(types, CompCount);
	requiredMask = BuildQueryMask(types, CompCount);

	return *this;
}

template<typename... Comps>
inline QueryDescription& QueryDescription::OneOrMore()
{
	constexpr u32 CompCount = sizeof...(Comps);
	static_assert(CompCount < MaxComponentsPerArchetype,
		"No Archetype has this many components. Check MaxComponentsPerArchetype for details");

	static const ComponentType* types[] = { GetComponentTypeFor<Comps>()... };
	static_assert(ArraySize(types) == CompCount, "Number of ComponentType ptrs is wrong");

	InsertionSort(types, CompCount);

	Assert(oneOrMoreComps.IsEmpty());
	oneOrMoreComps.Reserve(CompCount);
	oneOrMoreComps.AddRange(types, CompCount);
	oneOrMoreMask = BuildQueryMask(types, CompCount);

	return *this;
}

template<typename... Comps>
inline QueryDescription& QueryDescription::None()
{
	constexpr u32 CompCount = sizeof...(Comps);
	static_assert(CompCount < MaxComponentsPerArchetype,
		"No Archetype has this many components. Check MaxComponentsPerArchetype for details");

	static const ComponentType* types[] = { GetComponentTypeFor<Comps>()... };
	static_assert(ArraySize(types) == CompCount, "Number of ComponentType ptrs is wrong");

	InsertionSort(types, CompCount);

	Assert(noneComp.IsEmpty());
	noneComp.Reserve(CompCount);
	noneComp.AddRange(types, CompCount);
	noneMask = BuildQueryMask(types, CompCount);

	return *this;
}
}
