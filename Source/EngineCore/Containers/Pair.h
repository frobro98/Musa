// Copyright 2020, Nathan Blane

#pragma once

#include <type_traits>

#pragma warning(push)
#pragma warning(disable : 4820)

template <typename T1, typename T2>
struct Pair
{
	using FirstType = T1;
	using SecondType = T2;
	
	template <typename U1 = FirstType, typename U2 = SecondType,
		typename = std::enable_if_t<
			std::is_default_constructible_v<U1> &&
			std::is_default_constructible_v<U2>
		>>
	constexpr Pair()
		: first(), second()
	{
	}

	template<typename U1 = FirstType, typename U2 = SecondType,
		std::enable_if_t<std::conjunction_v<
			std::is_copy_constructible<U1>,
			std::is_copy_constructible<U2>
		>, int> = 0>
	constexpr Pair(const FirstType& val0, const SecondType& val1)
		: first(val0), second(val1)
	{
	}

	template <typename U1, typename U2,
		std::enable_if_t<std::conjunction_v<
			std::is_constructible<FirstType, U1>,
			std::is_constructible<SecondType, U2>
		>, int> = 0>
	constexpr Pair(U1&& val0, U2&& val1)
		: first(std::forward<U1>(val0)),
		second(std::forward<U2>(val1))
	{
	}

	template <typename U1, typename U2,
		std::enable_if_t<std::conjunction_v<
		std::is_constructible<FirstType, U1>,
		std::is_constructible<SecondType, U2>
	>, int> = 0>
	constexpr Pair(Pair<U1, U2>&& pair)
		: first(std::forward<U1>(pair.first)),
		second(std::forward<U2>(pair.second))
	{
	}

	template<typename Other1, typename Other2>
	constexpr Pair(const Pair<Other1, Other2>& pair)
		: first(pair.first), second(pair.second)
	{
	}

	template<typename Other1, typename Other2>
	Pair& operator=(const Pair<Other1, Other2>& pair)
	{
		if (this != &pair)
		{
			first = pair.first;
			second = pair.second;
		}
		return *this;
	}

	Pair& operator=(const Pair& pair)
	{
		if (this != &pair)
		{
			first = pair.first;
			second = pair.second;
		}
		return *this;
	}

	Pair& operator=(Pair&& pair)
	{
		if (this != &pair)
		{
			first = std::forward<FirstType>(pair.first);
			second = std::forward<SecondType>(pair.second);
		}
		return *this;
	}

	Pair(Pair&&) noexcept = default;
	Pair(const Pair&) = default;

	FirstType first;
	SecondType second;
};

template <typename T1, typename T2>
bool operator==(const Pair<T1, T2>& pair1, const Pair<T1, T2>& pair2)
{
	return pair1.first == pair2.first && pair1.second == pair2.second;
}



#pragma warning(pop)