#pragma once

#include <type_traits>
#include "Types/Intrinsics.hpp"
#include "Assertion.h"

template <typename ElemType, uint32 capacity>
class FixedArray
{
public:
	using valueType = ElemType;
	using referenceType = ElemType&;
	using pointerType = ElemType*;
public:
	FixedArray() = default;
	FixedArray(uint32 startSize);

	template<typename AddType>
	void Add(AddType&& elem);

	void Remove(ElemType& elem);
	void RemoveAt(uint32 index, uint32 count = 1);
	
	template<typename AddType>
	[[nodiscard]] bool TryAdd(AddType&& elem);

	void Resize(uint32 newSize);
	void Clear();


	[[nodiscard]] bool IsEmpty() const;
	[[nodiscard]] bool HasRoom() const;

	[[nodiscard]] uint32 Size() const;
	[[nodiscard]] constexpr uint32 Capacity() const;

	[[nodiscard]] ElemType* GetData();
	[[nodiscard]] const ElemType* GetData() const;

	// TODO - Find? Sort? Contains?


	ElemType& operator[](uint32 index);
	const ElemType& operator[](uint32 index) const;
	
private:
	void Destroy(uint32 index, uint32 count);

	template <typename U>
	std::enable_if_t<
		std::is_same_v<pointerType, U> &&
		!std::is_trivially_destructible_v<std::remove_pointer_t<U>>
	>
		DestroyRange(U start, U end);
	template <typename U>
	std::enable_if_t<
		std::is_same_v<pointerType, U> &&
		std::is_trivially_destructible_v<std::remove_pointer_t<U>>
	>
		DestroyRange(U /*start*/, U /*end*/);

	void MoveBackAt(uint32 index, uint32 count);

public:
	class Iterator final
	{
	public:
		Iterator(FixedArray<ElemType, capacity>& arr)
			: arrData(arr.GetData()),
			size(arr.Size())
		{
		}

		Iterator(nullptr_t)
			: arrData(nullptr),
			size(0)
		{
		}

		Iterator& operator++()
		{
			if (index < size-1)
			{
				++index;
			}
			else
			{
				arrData = nullptr;
			}
			return *this;
		}

		bool operator!=(const Iterator& other)
		{
			return arrData != other.arrData;
		}

		ElemType& operator*()
		{
			return arrData[index];
		}

	private:
		ElemType* arrData;
		const uint32 size;
		uint32 index = 0;
	};

private:
	friend Iterator begin(FixedArray<ElemType, capacity>& arr) { return Iterator(arr); }
	friend Iterator end(FixedArray<ElemType, capacity>&) { return Iterator(nullptr); }

private:
	ElemType data[capacity] = {};
	uint32 size = 0;
};

template<typename ElemType, uint32 capacity>
template<typename AddType>
inline void FixedArray<ElemType, capacity>::Add(AddType&& elem)
{
	Assert(size != capacity);
	data[size] = std::forward<AddType>(elem);
	++size;
}

template<typename ElemType, uint32 capacity>
template<typename AddType>
inline bool FixedArray<ElemType, capacity>::TryAdd(AddType&& elem)
{
	if (size < capacity)
	{
		Add(std::forward<AddType>(elem));
		return true;
	}
	return false;
}

template<typename ElemType, uint32 capacity>
inline FixedArray<ElemType, capacity>::FixedArray(uint32 startSize)
	: size(startSize)
{
	Assert(startSize <= capacity);
}

template<typename ElemType, uint32 capacity>
inline void FixedArray<ElemType, capacity>::Remove(ElemType& elem)
{
	for (uint32 i = 0; i < size; ++i)
	{
		if (data[i] == elem)
		{
			RemoveAt(i);
		}
	}
}

template<typename ElemType, uint32 capacity>
inline void FixedArray<ElemType, capacity>::RemoveAt(uint32 index, uint32 count)
{
	Assert(index + count < size);
	Destroy(index, count);
	MoveBackAt(index, count);
	size -= count;
}

template<typename ElemType, uint32 capacity>
inline void FixedArray<ElemType, capacity>::Resize(uint32 newSize)
{
	Assert(newSize <= capacity);
	if (size > newSize)
	{
		uint32 destroyCount = size - newSize;
		Destroy(newSize, destroyCount);
	}
	size = newSize;
}

template<typename ElemType, uint32 capacity>
inline void FixedArray<ElemType, capacity>::Clear()
{
	Destroy(0, size);
	size = 0;
}

template<typename ElemType, uint32 capacity>
inline bool FixedArray<ElemType, capacity>::IsEmpty() const
{
	return size != 0;
}

template<typename ElemType, uint32 capacity>
inline bool FixedArray<ElemType, capacity>::HasRoom() const
{
	return size < capacity;
}

template<typename ElemType, uint32 capacity>
inline uint32 FixedArray<ElemType, capacity>::Size() const
{
	return size;
}

template<typename ElemType, uint32 capacity>
inline constexpr uint32 FixedArray<ElemType, capacity>::Capacity() const
{
	return capacity;
}

template<typename ElemType, uint32 capacity>
inline ElemType* FixedArray<ElemType, capacity>::GetData()
{
	return data;
}

template<typename ElemType, uint32 capacity>
inline const ElemType * FixedArray<ElemType, capacity>::GetData() const
{
	return data;
}

template<typename ElemType, uint32 capacity>
inline ElemType& FixedArray<ElemType, capacity>::operator[](uint32 index)
{
	Assert(index < size);
	return data[index];
}

template<typename ElemType, uint32 capacity>
inline const ElemType& FixedArray<ElemType, capacity>::operator[](uint32 index) const
{
	Assert(index < size);
	return data[index];
}

template<typename ElemType, uint32 capacity>
inline void FixedArray<ElemType, capacity>::Destroy(uint32 index, uint32 count)
{
	Assert(index + count <= size);
	DestroyRange(&data[index], &data[index + count]);
	MoveBackAt(index, count);
}

template<typename ElemType, uint32 capacity>
template <typename U>
std::enable_if_t<
	std::is_same_v<ElemType*, U> &&
	!std::is_trivially_destructible_v<std::remove_pointer_t<U>>
>
inline FixedArray<ElemType, capacity>::DestroyRange(U start, U end)
{
	for (; start != end; ++start)
	{
		start->~Type();
	}
}

template<typename ElemType, uint32 capacity>
template <typename U>
std::enable_if_t<
	std::is_same_v<ElemType*, U> &&
	std::is_trivially_destructible_v<std::remove_pointer_t<U>>
>
inline FixedArray<ElemType, capacity>::DestroyRange(U /*start*/, U /*end*/)
{
}

template<typename ElemType, uint32 capacity>
inline void FixedArray<ElemType, capacity>::MoveBackAt(uint32 index, uint32 count)
{
	Assert(index + count <= size);
	const uint32 srcIndex = index + count;
	const uint32 dstIndex = index;
	pointerType srcLoc = &data[srcIndex];
	pointerType dstLoc = &data[dstIndex];
	size_t memSize = (size - srcIndex) * sizeof(valueType);
	Memmove(dstLoc, memSize, srcLoc, memSize);
}
