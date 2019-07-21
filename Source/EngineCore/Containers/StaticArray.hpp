#pragma once

#include "Types/Intrinsics.hpp"


template <class Type, uint32 size>
struct StaticArray
{
	using ValueType = Type;

	uint32 Size() const;

	ValueType internalData[size];
};