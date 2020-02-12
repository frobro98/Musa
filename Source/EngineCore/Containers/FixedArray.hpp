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
	template<typename AddType>
	void Add(AddType&& elem);
	void RemoveAt(uint32 index, uint32 count = 1);
	
	template<typename AddType>
	[[nodiscard]] bool TryAdd(AddType&& elem);

	[[nodiscard]] bool IsEmpty() const;

	[[nodiscard]] uint32 Size() const;
	[[nodiscard]] constexpr uint32 Capacity() const;

	[[nodiscard]] ElemType* GetData() const;

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
inline void FixedArray<ElemType, capacity>::RemoveAt(uint32 index, uint32 count)
{
	Assert(index + count < size);
	Destroy(index, count);
	MoveBackAt(index, count);
	size -= count;
}

template<typename ElemType, uint32 capacity>
inline bool FixedArray<ElemType, capacity>::IsEmpty() const
{
	return size != 0;
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
inline ElemType* FixedArray<ElemType, capacity>::GetData() const
{
	return data;
}

template<typename ElemType, uint32 capacity>
inline ElemType& FixedArray<ElemType, capacity>::operator[](uint32 index)
{
	return data[index];
}

template<typename ElemType, uint32 capacity>
inline const ElemType& FixedArray<ElemType, capacity>::operator[](uint32 index) const
{
	return data[index];
}

template<typename ElemType, uint32 capacity>
inline void FixedArray<ElemType, capacity>::Destroy(uint32 index, uint32 count)
{
	Assert(index + count < size);
	DestroyRange(data[index], data[index + count]);
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
	Assert(index + count < size);
	const uint32 srcIndex = index + count;
	const uint32 dstIndex = index;
	pointerType srcLoc = &data[srcIndex];
	pointerType dstLoc = &data[dstIndex];
	size_t memSize = (size - srcIndex) * sizeof(valueType);
	Memmove(dstLoc, memSize, srcLoc, memSize);
}
