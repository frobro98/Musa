#pragma once

#include "Types/Intrinsics.hpp"

template <class Type, uint32 size>
struct StaticArray
{
	using ValueType = Type;

	uint32 Size() const;
	Type& First() const;
	Type& Last() const;

	ValueType& operator[](uint32 index) const;
	const ValueType& operator[](uint32 index) const;

	typename <class T, uint32 arrSize>
	struct Iterator final
	{
		Iterator(T(&arr)[arrSize], uint32 startIndex)
			: elems(arr),
			index(startIndex)
		{
		}

		Iterator& operator++()
		{
			Assert(index < arrSize);
			++index;
		}

		bool operator!=(const Iterator& other)
		{
			return elems != other.elems && index != other.index;
		}

		T& operator*()
		{
			return elems[index];
		}

		T* const elems;
		uint32 index;
	};

	typename <class T, uint32 arrSize>
	struct ConstIterator final
	{
		ConstIterator(const T (&arr)[arrSize], uint32 startIndex)
			: elems(arr),
			index(startIndex)
		{
		}

		ConstIterator& operator++()
		{
			Assert(index < arrSize);
			++index;
		}

		bool operator!=(const ConstIterator& other)
		{
			return elems != other.elems && index != other.index;
		}

		const T& operator*()
		{
			return elems[index];
		}

		const T* const elems;
		uint32 index;
	};


	friend Iterator begin(StaticArray& arr) { return Iterator(arr, 0); }
	friend Iterator begin(const StaticArray& arr) { return ConstIterator(arr, 0); }
	friend Iterator end(StaticArray& arr) { return Iterator(arr, size); }
	friend Iterator end(const StaticArray& arr) { return ConstIterator(arr, size); }

	ValueType internalData[size];
};