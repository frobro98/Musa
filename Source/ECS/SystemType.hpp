// Copyright 2020, Nathan Blane

#pragma once

#include "ECS/Internal/TypenameHashing.hpp"
#include "ECS/Internal/robin_hood.hpp"

namespace Musa
{
class System;

struct SystemType
{
	uint64 typenameHash;
};

// TODO - I'm not entirely sure why this might be needed. Consider refactoring this out
static robin_hood::unordered_node_map<uint64, SystemType> systemTypeCache;

template <class Sys>
forceinline const SystemType MakeSystemTypeFor()
{
	static_assert(std::is_base_of_v<System, Sys>, "Type passed in as a template parameter must be derived from Musa::System");

	SystemType type = {};
	type.typenameHash = Musa::Internal::TypenameHash<Sys>();

	return type;
}

template <class Sys>
forceinline const SystemType* GetSystemTypeFor()
{
	static_assert(std::is_base_of_v<System, Sys>, "Type passed in as a template parameter must be derived from Musa::System");
	static const SystemType* type = []
	{
		constexpr uint64 typenameHash = Musa::Internal::template TypenameHash<Sys>();
		auto typeIter = systemTypeCache.find(typenameHash);
		if (typeIter == systemTypeCache.end())
		{
			const SystemType t = MakeSystemTypeFor<Sys>();
			systemTypeCache[typenameHash] = t;
		}
		// NOTE - This is an ok thing to do because the map has indirection built into it. References are stable
		return &systemTypeCache[typenameHash];
	}();

	return type;
}

forceinline const SystemType* GetSystemTypeFrom(uint64 typeKey)
{
	Assert(systemTypeCache.find(typeKey) != systemTypeCache.end());
	return &systemTypeCache[typeKey];
}

template <class Sys>
constexpr forceinline uint64 GetSystemTypeKeyFor()
{
	static_assert(std::is_base_of_v<System, Sys>, "Type passed in as a template parameter must be derived from Musa::System");
	return Musa::Internal::template TypenameHash<Sys>();
}

forceinline uint64 GetSystemTypeKeyFor(const SystemType* type)
{
	return type->typenameHash;
}
}
