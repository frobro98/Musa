#pragma once

#include "BasicTypes/Intrinsics.hpp"

template <typename T, uint32 N>
constexpr size_t ArraySize(NOT_USED const T(&arr)[N])
{
	return N;
}

