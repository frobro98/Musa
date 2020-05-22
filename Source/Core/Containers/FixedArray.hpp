// Copyright 2020, Nathan Blane

#pragma once

#include <type_traits>
#include "BasicTypes/Intrinsics.hpp"
#include "Debugging/Assertion.hpp"
#include "Utilities/MemoryUtilities.hpp"
#include "Utilities/TemplateUtils.hpp"
#include "CoreAPI.hpp"

template <typename ElemType, uint32 capacity>
class CORE_TEMPLATE FixedArray
{
	using value_type = ElemType;
	using reference_type = ElemType&;
	using pointer_type = ElemType*;
public:
	FixedArray() = default;
	FixedArray(uint32 startSize);

	template<typename AddType>
	void Add(AddType&& elem);
	void AddRange(const pointer_type range, uint32 rangeSize);

	void Remove(reference_type elem);
	void RemoveAt(uint32 index, uint32 count = 1);
	
	template<typename AddType>
	NODISCARD bool TryAdd(AddType&& elem);

	void Resize(uint32 newSize);
	void Clear();


	NODISCARD bool IsEmpty() const;
	NODISCARD bool HasRoom() const;

	NODISCARD uint32 Size() const;
	NODISCARD constexpr uint32 Capacity() const;

	NODISCARD ElemType* GetData();
	NODISCARD const ElemType* GetData() const;

	// TODO - Find? Sort? Contains?


	ElemType& operator[](uint32 index);
	const ElemType& operator[](uint32 index) const;
	
private:
	void Destroy(uint32 index, uint32 count);

	template <typename SrcType, typename DstType>
	std::enable_if_t<
		is_memcpy_constructable_v<SrcType, DstType>
	>
		ConstructRangeInPlace(DstType* dst, const SrcType* type, uint32 count);

	template <typename SrcType, typename DstType>
	std::enable_if_t<
		!is_memcpy_constructable_v<SrcType, DstType>
	>
		ConstructRangeInPlace(DstType* dst, const SrcType* type, uint32 count);

	template <typename U>
	std::enable_if_t<
		std::is_same_v<pointer_type, U> &&
		!std::is_trivially_destructible_v<std::remove_pointer_t<U>>
	>
		DestroyRange(U start, U end);
	template <typename U>
	std::enable_if_t<
		std::is_same_v<pointer_type, U> &&
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

	class ConstIterator final
	{
	public:
		ConstIterator(const FixedArray<ElemType, capacity>& arr)
			: arrData(arr.GetData()),
			size(arr.Size())
		{
		}

		ConstIterator(nullptr_t)
			: arrData(nullptr),
			size(0)
		{
		}

		ConstIterator& operator++()
		{
			if (index < size - 1)
			{
				++index;
			}
			else
			{
				arrData = nullptr;
			}
			return *this;
		}

		bool operator!=(const ConstIterator& other)
		{
			return arrData != other.arrData;
		}

		const ElemType& operator*()
		{
			return arrData[index];
		}

	private:
		const ElemType* arrData;
		const uint32 size;
		uint32 index = 0;
	};

private:
	friend Iterator begin(FixedArray<ElemType, capacity>& arr) { return Iterator(arr); }
	friend ConstIterator begin(const FixedArray<ElemType, capacity>& arr) { return ConstIterator(arr); }
	friend Iterator end(FixedArray<ElemType, capacity>&) { return Iterator(nullptr); }
	friend ConstIterator end(const FixedArray<ElemType, capacity>&) { return ConstIterator(nullptr); }

private:
	ElemType data[capacity] = {};
	uint32 size = 0;
};


template<typename ElemType, uint32 capacity>
inline FixedArray<ElemType, capacity>::FixedArray(uint32 startSize)
	: size(startSize)
{
	Assert(startSize <= capacity);
}

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
inline void FixedArray<ElemType, capacity>::AddRange(const pointer_type range, uint32 rangeSize)
{
	Assert(range);
	Assert(rangeSize > 0);
	Assert(size + rangeSize < capacity);

	uint32 index = size;
	ConstructRangeInPlace(data + index, range, rangeSize);
	size += rangeSize;
}

template<typename ElemType, uint32 capacity>
inline void FixedArray<ElemType, capacity>::Remove(reference_type elem)
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
template <typename SrcType, typename DstType>
std::enable_if_t<
	is_memcpy_constructable_v<SrcType, DstType>
>
inline FixedArray<ElemType, capacity>::ConstructRangeInPlace(DstType* dst, const SrcType* type, uint32 count)
{
	const uint32 byteCount = sizeof(SrcType) * count;
	Memcpy(dst, type, byteCount);
}

template<typename ElemType, uint32 capacity>
template <typename SrcType, typename DstType>
std::enable_if_t<
	!is_memcpy_constructable_v<SrcType, DstType>
>
inline FixedArray<ElemType, capacity>::ConstructRangeInPlace(DstType* dst, const SrcType* src, uint32 count)
{
	while (count > 0)
	{
		::new(dst) SrcType(*src);
		++src;
		++dst;
		++count;
	}
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
	pointer_type srcLoc = &data[srcIndex];
	pointer_type dstLoc = &data[dstIndex];
	size_t memSize = (size - srcIndex) * sizeof(value_type);
	Memmove(dstLoc, memSize, srcLoc, memSize);
}
