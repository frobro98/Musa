#pragma once

#include "Containers/DynamicArray.hpp"

// Data Structure that has hash map semantics in a linear, more memory/cache friendly structure
template <typename TypeA, typename TypeB>
class ListPair
{
public:

public:
	const DynamicArray<TypeA>& first;
	const DynamicArray<TypeB>& second;

private:
	DynamicArray<TypeA> firstList;
	DynamicArray<TypeB> secondList;
};

