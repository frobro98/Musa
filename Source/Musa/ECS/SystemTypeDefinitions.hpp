#pragma once

#include <type_traits>

#include "EngineCore/Types/Intrinsics.hpp"

namespace Musa
{
struct SystemType
{
	uint32 typeIndex;

	constexpr SystemType(uint32 index)
		: typeIndex(index)
	{}

	constexpr SystemType(const SystemType& other)
		: typeIndex(other.typeIndex)
	{}

	constexpr SystemType& operator=(const SystemType& other)
	{
		if (this != &other)
		{
			typeIndex = other.typeIndex;
		}

		return *this;
	}

	friend constexpr bool operator==(const SystemType& lhs, const SystemType& rhs)
	{
		return lhs.typeIndex == rhs.typeIndex;
	}

	friend constexpr bool operator!=(const SystemType& lhs, const SystemType& rhs)
	{
		return lhs.typeIndex != rhs.typeIndex;
	}

	friend constexpr bool operator<(const SystemType& lhs, const SystemType& rhs)
	{
		return lhs.typeIndex < rhs.typeIndex;
	}

	friend constexpr bool operator>(const SystemType& lhs, const SystemType& rhs)
	{
		return lhs.typeIndex > rhs.typeIndex;
	}

	friend constexpr bool operator<=(const SystemType& lhs, const SystemType& rhs)
	{
		return lhs.typeIndex <= rhs.typeIndex;
	}

	friend constexpr bool operator>=(const SystemType& lhs, const SystemType& rhs)
	{
		return lhs.typeIndex >= rhs.typeIndex;
	}
};

}

template<class Sys>
constexpr Musa::SystemType GetSystemType()
{
	static_assert(!std::is_same_v<Sys, Sys>, "Type for this System isn't defined! Must declare via DEFINE_SYSTEM");
}

#define DEFINE_SYSTEM(type)																								\
static_assert(std::is_base_of_v<Musa::System, type>, "Type passed into DEFINE_SYSTEM must derive from Component!");			\
static constexpr Musa::SystemType system_##type = {__COUNTER__};														\
template<>																												\
constexpr Musa::SystemType GetSystemType<type>()																		\
{																														\
	return system_##type;																								\
}

inline uint32 GetHash(Musa::SystemType type)
{
	return type.typeIndex;
}
