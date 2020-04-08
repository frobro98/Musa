#pragma once

#include "FNV-1a.h"
#include "ECS/Internal/robin_hood.hpp"
#include "ECS/Internal/TypenameHashing.hpp"

//namespace Musa
//{
using ComponentCtor = void(*)(void*);
using ComponentDtor = void(*)(void*);

struct ComponentType
{
	ComponentCtor ctor = nullptr;
	ComponentDtor dtor = nullptr;
	uint64 typenameHash = 0;
	uint64 archetypeBit = 0;
	uint16 size = 0;
	uint16 alignment = 0;
};

template <typename Comp>
static constexpr ComponentType MakeTypeFor()
{
	static_assert(!std::is_empty_v<Comp>, "A component can't be size 0");

	ComponentType type = {};
	type.typenameHash = Musa::Internal::TypenameHash<Comp>();
	type.archetypeBit |= 1ull << (type.typenameHash % 63ull);
	type.size = sizeof(Comp);
	type.alignment = alignof(Comp);

	type.ctor = [](void* ptr)
	{
		new(p) Comp;
	};
	type.dtor = [](void* ptr)
	{
		reinterpret_cast<Comp*>(ptr)->~Comp();
	};

	return type;
}

// TODO - I'm not entirely sure why this might be needed. Consider refactoring this out
static robin_hood::unordered_node_map<uint64, ComponentType> componentTypeCache;

template <typename Comp>
static const ComponentType* GetTypeFor()
{
	static const ComponentType* type = []()
	{
		constexpr typeHash = Internal::TypenameHash<Comp>();

		auto typeIter = componentTypeCache.find(typeHash);
		if (typeIter == componentTypeCache.end())
		{
			const ComponentType type = MakeTypeFor<Comp>();
			componentTypeCache[typeHash] = type;
		}

		// NOTE - This is an ok thing to do because the map has indirection built into it. References are stable
		return &componentTypeCache[typeHash];
	}();
	return type;
}

//}