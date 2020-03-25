#pragma once

#include <type_traits>

template <typename Type, typename... Types>
struct all_same_type
{
	constexpr bool IsAllSame()
	{
		bool result = true;
		[[maybe_unused]] int dummy[] = { 0, (result = result && std::is_same_v<Type, Types>, 0)... };
		return result;
	}
	//static constexpr bool value = (std::is_same_v<Type, Types> && ...);
	static constexpr bool value = IsAllSame();
};

template <typename Type, typename... Types>
inline constexpr bool all_same_type_v = all_same_type<Type, Types...>::value;

template <typename Base, typename... Derived>
struct all_base_of
{
	static constexpr bool IsAllBase()
	{
		bool result = true;
		[[maybe_unused]] int dummy[] = { 0, (result = result && std::is_base_of_v<Base, Derived>, 0)... };
		return result;
	}
	static constexpr bool value = IsAllBase();
};

template <typename Base, typename... Derived>
inline constexpr bool all_base_of_v = all_base_of<Base, Derived...>::value;

template<typename ConvertTo, typename... ConvertFrom>
struct all_convertable_to
{
	static constexpr bool AreAllConvertable()
	{
		bool result = true;
		[[maybe_unused]] int dummy[] = { 0, (result = result && std::is_convertible_v<ConvertFrom, ConvertTo>, 0)... };
		return result;
	}

	static constexpr bool value = AreAllConvertable();
};

template<typename ConvertTo, typename... ConvertFrom>
inline constexpr bool all_convertable_to_v = all_convertable_to<ConvertTo, ConvertFrom...>::value;
