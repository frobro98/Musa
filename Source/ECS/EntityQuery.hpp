#pragma once

#include "Containers/DynamicArray.hpp"
#include "ECS/ComponentType.hpp"

namespace Musa
{
struct EntityQuery
{
	template <typename... Comps>
	EntityQuery& With();
	template <typename... Comps>
	EntityQuery& Any();
	template <typename... Comps>
	EntityQuery& None();

	void Pack();

	DynamicArray<ComponentTypeHash> withCompDesc;
	uint64 withArchetypeId;
	DynamicArray<ComponentTypeHash> anyCompDesc;
	uint64 anyArchetypeId;
	DynamicArray<ComponentTypeHash> noneCompDesc;
	uint64 noneArchetypeId;
};
}
