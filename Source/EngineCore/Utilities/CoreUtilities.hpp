#pragma once

#include <algorithm>

template<typename T>
constexpr T Max(T lhs, T rhs)
{
	return std::max(lhs, rhs);
}

template<typename T>
constexpr T Min(T lhs, T rhs)
{
	return std::min(lhs, rhs);
}

template<typename T>
constexpr T Clamp(T val, T minVal, T maxVal)
{
	return Min(Max(val, minVal), maxVal);
}