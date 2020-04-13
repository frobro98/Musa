#pragma once

#include "Containers/DynamicArray.hpp"

template <typename Exch>
void Exchange(Exch& e0, Exch& e1)
{
	Exch tmp = e0;
	e0 = e1;
	e1 = tmp;
}

// TODO - Combine the 2 different implementations for insertion sort
template<class Elem>
void InsertionSort(DynamicArray<Elem>& arr)
{
	int32 size = static_cast<int32>(arr.Size());
	for (int32 i = 1; i < size; ++i)
	{
		for (int32 j = i - 1; j >= 0; --j)
		{
			uint32 index = static_cast<uint32>(j);
			if (arr[index + 1] < arr[index])
			{
				Exchange(arr[index], arr[index + 1]);
			}
		}
	}
}

template <typename T, size_t N>
void InsertionSort(T(&arr)[N])
{
	for (size_t i = 1; i < N; ++i)
	{
		int64 index = (int64)i;
		for (int64 j = index - 1; j >= 0; --j)
		{
			if (arr[j + 1] < arr[j])
			{
				Exchange(arr[j], arr[j + 1]);
			}
		}
	}
}