#pragma once

#include <type_traits>

template <typename Type, typename... Types>
struct all_same_type
{
	static constexpr bool value = (std::is_same_v<Type, Types> && ...);
};

template <typename Type, typename... Types>
inline constexpr bool all_same_type_v = all_same_type<Type, Types...>::value;

template <typename Base, typename... Derived>
struct all_base_of
{
	static constexpr bool value = (std::is_base_of_v<Base, Derived> && ...);
};

template <typename Base, typename... Derived>
inline constexpr bool all_base_of_v = all_base_of<Base, Derived...>::value;