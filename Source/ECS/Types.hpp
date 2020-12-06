// Copyright 2020, Nathan Blane

#pragma once

#include <type_traits>

#include "ECS/Component.hpp"

using EntityIndex = u32;
using EntityVersion = u32;

using ArchetypeMask = u64;

constexpr u32 MaxComponentsPerArchetype = 32;

template <typename CompType>
struct is_valid_component_type
{
	static constexpr bool IsValidType()
	{
		return //std::conjunction_v<
			std::is_base_of_v<Musa::Component, CompType>;
			//, std::is_trivially_copyable<CompType>
			//, std::is_standard_layout<CompType>
		//>;
	}

	static constexpr bool value = IsValidType();
};

template <typename CompType>
inline constexpr bool is_valid_component_type_v = is_valid_component_type<CompType>::value;

template <typename CompType>
inline constexpr bool can_attach_to_entity_v = std::conjunction_v<
												std::is_base_of<Musa::Component, CompType>,
												is_valid_component_type<CompType>
											>;

template <typename... CompTypes>
struct all_valid_component_types
{
	static constexpr bool IsAllValid()
	{
		bool result = true;
		NOT_USED int dummy[] = { 0, (result = result && is_valid_component_type_v<CompTypes>, 0)... };
		return result;
	}

	static constexpr bool value = IsAllValid();
};

template <typename... CompTypes>
inline constexpr bool all_valid_component_types_v = all_valid_component_types<CompTypes...>::value;

template <typename... CompTypes>
struct all_can_attach_to_entity
{
	static constexpr bool IsAllValid()
	{
		bool result = true;
		NOT_USED int dummy[] = { 0, (result = result && can_attach_to_entity_v<CompTypes>, 0)... };
		return result;
	}

	static constexpr bool value = IsAllValid();
};

template <typename... CompTypes>
inline constexpr bool all_can_attach_to_entity_v = all_can_attach_to_entity<CompTypes...>::value;

