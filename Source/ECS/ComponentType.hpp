// Copyright 2020, Nathan Blane

#pragma once

#pragma warning(push)
#pragma warning(disable:4307)

#include "Utilities/HashFuncs.hpp"
#include "ECS/Internal/robin_hood.hpp"
#include "ECS/Internal/TypenameHashing.hpp"

namespace Musa
{
struct Component;

using ComponentCtor = void(*)(void*);
using ComponentDtor = void(*)(void*);

struct ComponentTypeHash
{
	uint64 typenameHash;
	uint64 archetypeBit;
};

forceinline bool operator==(const ComponentTypeHash& lhs, const ComponentTypeHash& rhs)
{
	return lhs.typenameHash == rhs.typenameHash && lhs.archetypeBit == rhs.archetypeBit;
}

struct ComponentType
{
	ComponentCtor ctor = nullptr;
	ComponentDtor dtor = nullptr;
	ComponentTypeHash hash;
	uint16 size = 0;
	uint16 alignment = 0;
};

template <typename Comp>
forceinline ComponentType MakeComponentTypeFor()
{
	static_assert(std::is_base_of_v<Component, Comp>, "Type passed in as a template parameter must be derived from Musa::Component");
	static_assert(!std::is_empty_v<Comp>, "A component can't be size 0");

	ComponentType type = {};

	constexpr uint64 hash = Musa::Internal::TypenameHash<Comp>();
	type.hash = {
		hash,
		1ull << (hash % 63ull)
	};
	type.size = sizeof(Comp);
	type.alignment = alignof(Comp);

	type.ctor = [](void* ptr)
	{
		new(ptr) Comp;
	};
	type.dtor = [](void* ptr)
	{
		reinterpret_cast<Comp*>(ptr)->~Comp();
	};

	return type;
}

// TODO - This should stay around, but there needs to be another way to get at the component types. Perhaps as an index?
static robin_hood::unordered_node_map<uint64, ComponentType> componentTypeCache;

template <typename Comp>
forceinline const ComponentType* GetComponentTypeFor()
{
	static_assert(std::is_base_of_v<Component, Comp>, "Type passed in as a template parameter must be derived from Musa::Component");

	static const ComponentType* type = []
	{
		constexpr uint64 typeHash = Musa::Internal::template TypenameHash<Comp>();

		auto typeIter = componentTypeCache.find(typeHash);
		if (typeIter == componentTypeCache.end())
		{
			const ComponentType t = MakeComponentTypeFor<Comp>();
			componentTypeCache[typeHash] = t;
		}

		// NOTE - This is an ok thing to do because the map has indirection built into it. References are stable
		return &componentTypeCache[typeHash];
	}();
	return type;
}

forceinline const ComponentType* GetComponentTypeFrom(uint64 typeKey)
{
	Assert(componentTypeCache.find(typeKey) != componentTypeCache.end());
	return &componentTypeCache[typeKey];
}

template <typename Comp>
constexpr forceinline uint64 GetComponentTypeKeyFor()
{
	static_assert(std::is_base_of_v<Component, Comp>, "Type passed in as a template parameter must be derived from Musa::Component");
	return Musa::Internal::template TypenameHash<Comp>();
}

forceinline uint64 GetComponentTypeKeyFor(const ComponentType* type)
{
	return type->hash.typenameHash;
}

}
#pragma warning(pop)

