#pragma once

#include "Containers/Array.h"

template <typename Exch>
void Exchange(Exch& e0, Exch& e1)
{
	Exch tmp = e0;
	e0 = e1;
	e1 = tmp;
}

template<class Elem>
void InsertionSort(Array<Elem>& arr)
{
	int32 size = static_cast<int32>(arr.Size());
	for (int32 i = 1; i < size; ++i)
	{
		for (int32 j = i - 1; j > 0; --j)
		{
			uint32 index = static_cast<uint32>(j);
			if (arr[index + 1] < arr[index])
			{
				Exchange(arr[index], arr[index + 1]);
			}
		}
	}
}