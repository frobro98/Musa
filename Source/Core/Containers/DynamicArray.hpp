// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "Debugging/Assertion.hpp"
#include "Utilities/MemoryUtilities.hpp"
#include "Serialization/SerializeBase.hpp"
#include "Serialization/DeserializeBase.hpp"
#include "Utilities/TemplateUtils.hpp"
#include "CoreAPI.hpp"

template<class Type>
class CORE_TEMPLATE DynamicArray
{
	using valueType = Type;
	using pointerType = Type*;
	using referenceType = Type&;

	// Required because other template types are different DynamicArray types
	template<typename OtherType>
	friend class DynamicArray;

public:
	DynamicArray();
	explicit DynamicArray(uint32 initialSize);
	DynamicArray(const Type* arr, uint32 elementCount);
	DynamicArray(std::initializer_list<Type> list);

	template <size_t N>
	DynamicArray(const Type(&arr)[N]);

	~DynamicArray();

	DynamicArray(const DynamicArray& otherArr);
	DynamicArray(DynamicArray&& otherArr) noexcept;

	template <class OtherType>
	DynamicArray(const DynamicArray<OtherType>& otherArr);

	DynamicArray& operator=(const DynamicArray& otherArr);
	DynamicArray& operator=(DynamicArray&& otherArr) noexcept;

	template <typename AddType>
	uint32 Add(AddType&& newElement);
	template <typename AddType>
	uint32 AddUnique(AddType&& newElement);

	// Creates an empty element in the array. Element isn't initialized in any way
	uint32 AddEmpty(uint32 emptyElements = 1);
	uint32 AddDefault(uint32 emptyElements = 1);
	uint32 AddRange(const pointerType range, uint32 rangeSize);

	template<class... Args>
	uint32 Emplace(Args... args);

	template <typename InsertType>
	void Insert(InsertType&& elem, uint32 index);

	bool TryRemoveElement(const valueType& elem);
	void Remove(uint32 index, uint32 count = 1);
	void RemoveFirst(const valueType& elem);
	void RemoveLast(const valueType& elem);
	void RemoveAll(const valueType& elem);

	template<class Pred>
	int32 FindFirstIndexUsing(Pred&& pred);
	template<class Pred>
	int32 FindLastIndexUsing(Pred&& pred);
	template<class Pred>
	valueType* FindFirstUsing(Pred&& pred);
	template<class Pred>
	valueType* FindLastUsing(Pred&& pred);

	int32 FindFirstIndex(const valueType& obj);
	int32 FindLastIndex(const valueType& obj);
	valueType* FindFirst(const valueType& obj);
	valueType* FindLast(const valueType& obj);

	bool TryFindFirstIndex(const valueType& obj, uint32& foundIndex);
	bool TryFindLastIndex(const valueType& obj, uint32& foundIndex);
	bool TryFindFirst(const valueType& obj, valueType& foundVal);
	bool TryFindLast(const valueType& obj, valueType& foundVal);

	void Reserve(uint32 reserveCapacity);
	// TODO - Figure out a better way of going about implementing this resize function...
	void Resize(uint32 newSize);
	void Clear();
	bool IsEmpty() const;
	template <typename CompareType>
	bool Contains(const CompareType& obj) const;

	template <typename Pred>
	void Sort(const Pred& predicate);

	inline Type* GetData()
	{
		return data;
	}

	inline const Type* GetData() const
	{
		return data;
	}

	inline uint32 Size() const
	{
		return arraySize;
	}

	inline uint32 SizeInBytes() const
	{
		return arraySize * sizeof(valueType);
	}

	inline uint32 Capacity() const
	{
		return arrayCapacity;
	}

	// Accessors
	inline Type& operator[](uint32 index)
	{
		Assert(index < arraySize);
		return GetData()[index];
	}

	inline const Type& operator[](uint32 index) const
	{
		Assert(index < arraySize);
		return GetData()[index];
	}

	inline valueType& First() const
	{
		Assert(arraySize > 0);
		return data[0];
	}

	inline valueType& Last() const
	{
		Assert(arraySize > 0);
		return data[arraySize - 1];
	}

public:
	// Dynamic Array Iterator
	class Iterator final
	{
	public:
		Iterator(DynamicArray<Type>& arr_)
			: arr(arr_.Size() > 0 ? &arr_ : nullptr)
		{
		}

		Iterator(nullptr_t)
			: arr(nullptr)
		{
		}

		Iterator& operator++()
		{
			if (arr != nullptr && currentIndex < arr->Size() - 1)
			{
				++currentIndex;
			}
			else
			{
				arr = nullptr;
			}
			return *this;
		}
		bool operator!=(const Iterator& other)
		{
			return arr != other.arr;
		}

		Type& operator*()
		{
			return (*arr)[currentIndex];
		}

	private:
		DynamicArray<Type>* arr;
		uint32 currentIndex = 0;
	};
	// Dynamic Array Constant Iterator
	class ConstIterator final
	{
	public:
		ConstIterator(const DynamicArray<Type>& arr_)
			: arr(arr_.Size() > 0 ? &arr_ : nullptr)
		{
		}

		ConstIterator(nullptr_t)
			: arr(nullptr)
		{
		}

		ConstIterator& operator++()
		{
			if (arr != nullptr && currentIndex < arr->Size() - 1)
			{
				++currentIndex;
			}
			else
			{
				arr = nullptr;
			}
			return *this;
		}
		bool operator!=(const ConstIterator& other)
		{
			return arr != other.arr;
		}

		const Type& operator*() const
		{
			return (*arr)[currentIndex];
		}

	private:
		const DynamicArray<Type>* arr;
		uint32 currentIndex = 0;
		uint32 pad[1] = { 0 };
	};

private:
	friend Iterator begin(DynamicArray& arr) { return Iterator(arr); }
	friend ConstIterator begin(const DynamicArray& arr) { return ConstIterator(arr); }
	friend Iterator end(DynamicArray& /*arr*/) { return Iterator(nullptr); }
	friend ConstIterator end(const DynamicArray& /*arr*/) { return ConstIterator(nullptr); }

	friend void Serialize(SerializeBase& ser, const DynamicArray& arr)
	{
		Serialize(ser, arr.arraySize);

		for (uint32 i = 0; i < arr.arraySize; ++i)
		{
			Serialize(ser, arr.data[i]);
		}
	}

	friend void Deserialize(DeserializeBase& ser, DynamicArray& arr)
	{
		uint32 size;
		Deserialize(ser, size);

		arr.Resize(size);
		for (uint32 i = 0; i < arr.arraySize; ++i)
		{
			Deserialize(ser, arr[i]);
		}
	}

private:
	template <class OtherType>
	void CopyData(OtherType* otherData, uint32 dataNum);
	void Construct(uint32 index);
	void Construct(uint32 startIndex, uint32 endIndex);

	// TODO - consider moving these operations out so more things could use them?
	template <typename U>
	std::enable_if_t<
		std::is_same_v<pointerType, U> &&
		std::is_scalar_v<std::remove_pointer_t<U>>
	>
		ConstructRange(U start, U end);
	template <typename U>
	std::enable_if_t<
		std::is_same_v<pointerType, U> &&
		!std::is_scalar_v<std::remove_pointer_t<U>>
	>
		ConstructRange(U start, U end);

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

	void Destroy(uint32 index);
	void Destroy(uint32 startIndex, uint32 endIndex);

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

	void MoveForward(uint32 startIndex, uint32 count = 1);
	void MoveBack(uint32 startIndex, uint32 count = 1);

	void AdjustSizeGeom(uint32 newSize);
	void CreateAdjustedSpace();

private:
	Type* data = nullptr;
	uint32 arraySize = 0;
	uint32 arrayCapacity = 0;
};

template<class Type>
DynamicArray<Type>::DynamicArray()
	: data(nullptr),
	arraySize(0),
	arrayCapacity(0)
{
}

template<class Type>
DynamicArray<Type>::DynamicArray(uint32 initialSize)
	:arraySize(initialSize),
	arrayCapacity(initialSize)
{
	if (initialSize > 0)
	{
		data = reinterpret_cast<Type*>(malloc(sizeof(Type) * initialSize));
		Construct(0, arraySize);
	}
}

template<class Type>
DynamicArray<Type>::DynamicArray(const Type* arr, uint32 elementCount)
	: arraySize(elementCount),
	arrayCapacity(elementCount)
{
	CopyData(arr, elementCount);
}

template<class Type>
DynamicArray<Type>::DynamicArray(std::initializer_list<Type> list)
	: arraySize(0),
	arrayCapacity(0)
{
	Reserve((uint32)list.size());
	const Type* beginning = list.begin();
	const Type* ending = list.end();
	for (; beginning != ending; ++beginning)
	{
		Add(*beginning);
	}
}

template<class Type>
template <size_t N>
DynamicArray<Type>::DynamicArray(const Type(&arr)[N])
	: arraySize(N),
	arrayCapacity(N)
{
	CopyData(arr, N);
}

template<class Type>
DynamicArray<Type>::~DynamicArray()
{
	DestroyRange(data, data + arraySize);

	free(data);
	arrayCapacity = 0;
	arraySize = 0;
}

template<class Type>
DynamicArray<Type>::DynamicArray(const DynamicArray& otherArr)
{
	arrayCapacity = otherArr.arraySize;
	CopyData(otherArr.data, otherArr.arraySize);
}

template<class Type>
template <class OtherType>
DynamicArray<Type>::DynamicArray(const DynamicArray<OtherType>& otherArr)
{
	CopyData(otherArr.GetData(), otherArr.arraySize);
	arrayCapacity = otherArr.arrayCapacity;
}

template<class Type>
DynamicArray<Type>::DynamicArray(DynamicArray&& otherArr) noexcept
{
	DestroyRange(data, data + arraySize);
	free(data);

	data = otherArr.data;
	arraySize = otherArr.arraySize;
	arrayCapacity = otherArr.arrayCapacity;

	otherArr.data = nullptr;
	otherArr.arraySize = 0;
	otherArr.arrayCapacity = 0;
}

template<class Type>
DynamicArray<Type>& DynamicArray<Type>::operator=(const DynamicArray& otherArr)
{
	if (this != &otherArr)
	{
		CopyData(otherArr.GetData(), otherArr.arraySize);
		arrayCapacity = otherArr.arrayCapacity;
	}

	return *this;
}

template<class Type>
DynamicArray<Type>& DynamicArray<Type>::operator=(DynamicArray&& otherArr) noexcept
{
	if (this != &otherArr)
	{
		DestroyRange(data, data + arraySize);
		free(data);

		data = otherArr.data;
		arraySize = otherArr.arraySize;
		arrayCapacity = otherArr.arrayCapacity;

		otherArr.data = nullptr;
		otherArr.arraySize = 0;
		otherArr.arrayCapacity = 0;
	}

	return *this;
}

template<class Type>
template <typename AddType>
inline uint32 DynamicArray<Type>::Add(AddType&& newElement)
{
	uint32 newSize = arraySize + 1;
	if (newSize > arrayCapacity)
	{
		AdjustSizeGeom(newSize);
	}

	return Emplace(std::forward<AddType>(newElement));
}

template<class Type>
template<typename AddType>
inline uint32 DynamicArray<Type>::AddUnique(AddType&& newElement)
{
	int32 index = FindFirstIndex(newElement);
	if (index < 0)
	{
		index = Add(std::forward<AddType>(newElement));
	}
	return (uint32)index;
}

template<class Type>
inline uint32 DynamicArray<Type>::AddEmpty(uint32 emptyElements)
{
	uint32 newSize = arraySize + emptyElements;
	if (newSize > arrayCapacity)
	{
		AdjustSizeGeom(newSize);
	}

	uint32 index = arraySize;
	arraySize = newSize;
	return index;
}

template<class Type>
inline uint32 DynamicArray<Type>::AddDefault(uint32 emptyElements)
{
	uint32 newSize = arraySize + emptyElements;
	if (newSize > arrayCapacity)
	{
		AdjustSizeGeom(newSize);
	}

	uint32 index = arraySize;
	arraySize = newSize;
	Construct(index, arraySize);
	return index;
}

template<class Type>
inline uint32 DynamicArray<Type>::AddRange(const pointerType range, uint32 rangeSize)
{
	Assert(range);
	Assert(rangeSize > 0);

	uint32 index = AddEmpty(rangeSize);
	ConstructRangeInPlace(data + index, range, rangeSize);
	return index;
}

template<class Type>
template <typename InsertType>
inline void DynamicArray<Type>::Insert(InsertType&& elem, uint32 index)
{
	uint32 newSize = arraySize + 1;
	if (newSize > arrayCapacity)
	{
		AdjustSizeGeom(newSize);
	}

	MoveForward(index);
	new(GetData() + index) valueType(std::forward<InsertType>(elem));
	arraySize = newSize;
}

template<class Type>
template<typename Pred>
inline void DynamicArray<Type>::Sort(const Pred& pred)
{
	// TODO - Test this number and see when quick sort is faster than insertion sort
	if (arraySize <= 30)
	{
		// Insertion sort
		for (uint32 i = 1; i < arraySize; ++i)
		{
			for (int32 j = i; j > 0; --j)
			{
				if (pred(data[j], data[j - 1]))
				{
					// Exchange
					Type tmp = data[j];
					data[j] = data[j - 1];
					data[j - 1] = tmp;

					break;
				}
			}
		}
	}
	else
	{
		// Quicksort
		// TODO - Implement Quicksort
	}
}

template<class Type>
inline bool DynamicArray<Type>::TryRemoveElement(const valueType& elem)
{
	bool result = false;
	for (uint32 i = 0; i < arraySize; ++i)
	{
		if (GetData()[i] == elem)
		{
			Remove(i);
			result = true;
			break;
		}
	}

	return result;
}

template<class Type>
inline void DynamicArray<Type>::Remove(uint32 index, uint32 count)
{
	Assert(index < arraySize);
	Destroy(index, index + count);
	MoveBack(index + count, count);
	arraySize -= count;
}

template<class Type>
inline void DynamicArray<Type>::RemoveFirst(const valueType& elem)
{
	for (uint32 i = 0; i < arraySize; ++i)
	{
		if (GetData()[i] == elem)
		{
			Remove(i);
			break;
		}
	}
}

template<class Type>
inline void DynamicArray<Type>::RemoveLast(const valueType& elem)
{
	for (int32 i = arraySize - 1; i >= 0; --i)
	{
		uint32 index = (uint32)i;
		if (GetData()[index] == elem)
		{
			Remove(index);
			break;
		}
	}
}

template<class Type>
inline void DynamicArray<Type>::RemoveAll(const valueType& elem)
{
	for (uint32 i = 0; i < arraySize; ++i)
	{
		if (GetData()[i] == elem)
		{
			Remove(i);
		}
	}
}

template<class Type>
template<class Pred>
inline int32 DynamicArray<Type>::FindFirstIndexUsing(Pred&& pred)
{
	int32 foundIndex = -1;
	for (uint32 i = 0; i < arraySize; ++i)
	{
		if (pred(data[i]))
		{
			foundIndex = (int32)i;
		}
	}
	return foundIndex;
}

template<class Type>
template<class Pred>
inline int32 DynamicArray<Type>::FindLastIndexUsing(Pred&& pred)
{
	int32 foundIndex = -1;
	for (int32 i = static_cast<int32>(arraySize - 1); i > 0; --i)
	{
		if (pred(data[i]))
		{
			foundIndex = i;
		}
	}
	return foundIndex;
}

template<class Type>
template<class Pred>
inline Type* DynamicArray<Type>::FindFirstUsing(Pred&& pred)
{
	valueType* found = nullptr;
	int32 foundIndex = FindFirstIndexUsing(std::move(pred));
	if (foundIndex >= 0)
	{
		found = &data[(uint32)foundIndex];
	}

	return found;
}

template<class Type>
template<class Pred>
inline Type* DynamicArray<Type>::FindLastUsing(Pred&& pred)
{
	valueType* found = nullptr;
	int32 foundIndex = FindLastIndex(std::move(pred));
	if (foundIndex >= 0)
	{
		found = &data[(uint32)foundIndex];
	}

	return found;
}

template<class Type>
inline int32 DynamicArray<Type>::FindFirstIndex(const valueType& obj)
{
	return FindFirstIndexUsing([&](const valueType& o) {return obj == o; });
}

template<class Type>
inline int32 DynamicArray<Type>::FindLastIndex(const valueType& obj)
{
	return FindLastIndexUsing([&](const valueType& o) {return obj == o; });
}

template<class Type>
inline Type* DynamicArray<Type>::FindFirst(const valueType& obj)
{
	return FindFirstUsing([&](const valueType& o) {return o == obj; });
}

template<class Type>
inline Type* DynamicArray<Type>::FindLast(const valueType& obj)
{
	return FindLastUsing([&](const valueType& o) {return obj == o; });
}

template<class Type>
inline bool DynamicArray<Type>::TryFindFirstIndex(const valueType& obj, uint32& foundIndex)
{
	bool found = false;
	for (uint32 i = 0; i < arraySize; ++i)
	{
		if (data[i] == obj)
		{
			foundIndex = i;
			found = true;
			break;
		}
	}
	return found;
}

template<class Type>
inline bool DynamicArray<Type>::TryFindLastIndex(const valueType& obj, uint32& foundIndex)
{
	bool found = false;
	for (int32 i = static_cast<int32>(arraySize - 1); i > 0; --i)
	{
		if (data[i] == obj)
		{
			foundIndex = i;
			found = true;
			break;
		}
	}
	return found;
}

template<class Type>
inline bool DynamicArray<Type>::TryFindFirst(const valueType& obj, valueType& foundVal)
{
	uint32 i;
	bool found = TryFindFirstIndex(obj, i);
	if (found)
	{
		foundVal = data[i];
	}

	return found;
}

template<class Type>
inline bool DynamicArray<Type>::TryFindLast(const valueType& obj, valueType& foundVal)
{
	uint32 i;
	bool found = TryFindLastIndex(obj, i);
	if (found)
	{
		foundVal = data[i];
	}

	return found;
}

template<class Type>
inline void DynamicArray<Type>::Reserve(uint32 reserveCapacity)
{
	if (arrayCapacity < reserveCapacity)
	{
		arrayCapacity = reserveCapacity;
		CreateAdjustedSpace();
	}
}

// TODO - Figure out a better way of going about implementing this resize function...
template<class Type>
inline void DynamicArray<Type>::Resize(uint32 newSize)
{
	if (newSize != arraySize)
	{
		if (newSize < arraySize)
		{
			const uint32 sizeDiff = arraySize - newSize;
			Destroy(newSize, arraySize);
			arraySize = newSize;

		}
		else
		{
			if (newSize > arrayCapacity)
			{
				arrayCapacity = newSize;
				CreateAdjustedSpace();

				uint32 oldSize = arraySize;
				arraySize = newSize;
				Construct(oldSize, newSize);
			}
			else
			{

				uint32 oldSize = arraySize;
				arraySize = newSize;
				Construct(oldSize, newSize);
			}
		}
	}
}

template<class Type>
inline void DynamicArray<Type>::Clear()
{
	if (arraySize > 0)
	{
		Destroy(0, arraySize);
		arraySize = 0;
	}
}

template<class Type>
inline bool DynamicArray<Type>::IsEmpty() const
{
	return arraySize == 0;
}

template<class Type>
template<class... Args>
inline uint32 DynamicArray<Type>::Emplace(Args... args)
{
	uint32 index = AddEmpty();
	new(GetData() + index) Type(std::forward<Args>(args)...);
	return index;
}

template<class Type>
template<typename CompareType>
inline bool DynamicArray<Type>::Contains(const CompareType& obj) const
{
	bool found = false;
	for (uint32 i = 0; i < arraySize; ++i)
	{
		if (data[i] == obj)
		{
			found = true;
			break;
		}
	}
	return found;
}

//////////////////////////////////////////////////////////////////////////
// Private Helpers
//////////////////////////////////////////////////////////////////////////
template<class Type>
template <class OtherType>
inline void DynamicArray<Type>::CopyData(OtherType* otherData, uint32 dataNum)
{
	if (dataNum > 0)
	{
		arraySize = dataNum;
		data = reinterpret_cast<Type*>(malloc(dataNum * sizeof(Type)));
		Type* ptrIndex = data;
		while (dataNum > 0)
		{
			::new(ptrIndex) Type(*otherData);
			++ptrIndex;
			++otherData;
			--dataNum;
		}
	}
}

template<class Type>
inline void DynamicArray<Type>::Construct(uint32 index)
{
	Assert(index >= 0 && index < arraySize);
	ConstructRange(data + index, data + (index + 1));
}

template<class Type>
inline void DynamicArray<Type>::Construct(uint32 startIndex, uint32 endIndex)
{
	Assert(startIndex < arraySize);
	Assert(endIndex >= startIndex && endIndex <= arraySize);
	ConstructRange(data + startIndex, data + endIndex);
}

template<class Type>
template <typename U>
std::enable_if_t<
	std::is_same_v<Type*, U> &&
	std::is_scalar_v<std::remove_pointer_t<U>>
>
inline DynamicArray<Type>::ConstructRange(U start, U end)
{
	uint8* const beginning = reinterpret_cast<uint8*>(start);
	uint8* const ending = reinterpret_cast<uint8*>(end);
	Memset(beginning, 0, static_cast<size_t>(ending - beginning));
}

template<class Type>
template <typename U>
std::enable_if_t<
	std::is_same_v<Type*, U> &&
	!std::is_scalar_v<std::remove_pointer_t<U>>
>
inline DynamicArray<Type>::ConstructRange(U start, U end)
{
	for (; start != end; ++start)
	{
		new(static_cast<void*>(start)) Type();
	}
}

template<class Type>
template <typename SrcType, typename DstType>
std::enable_if_t<
	is_memcpy_constructable_v<SrcType, DstType>
>
inline DynamicArray<Type>::ConstructRangeInPlace(DstType* dst, const SrcType* type, uint32 count)
{
	const uint32 byteCount = sizeof(SrcType) * count;
	Memcpy(dst, type, byteCount);
}

template<class Type>
template <typename SrcType, typename DstType>
std::enable_if_t<
	!is_memcpy_constructable_v<SrcType, DstType>
>
	inline DynamicArray<Type>::ConstructRangeInPlace(DstType* dst, const SrcType* src, uint32 count)
{
	while (count > 0)
	{
		::new(dst) SrcType(*src);
		++src;
		++dst;
		++count;
	}
}

template<class Type>
inline void DynamicArray<Type>::Destroy(uint32 index)
{
	Assert(index >= 0 && index < arraySize);
	DestroyRange(data + index, data + (index + 1));
}

template<class Type>
inline void DynamicArray<Type>::Destroy(uint32 startIndex, uint32 endIndex)
{
	Assert(startIndex < arraySize);
	Assert(endIndex >= startIndex && endIndex <= arraySize);
	DestroyRange(data + startIndex, data + endIndex);
}

template<class Type>
template <typename U>
std::enable_if_t<
	std::is_same_v<Type*, U> &&
	!std::is_trivially_destructible_v<std::remove_pointer_t<U>>
>
inline DynamicArray<Type>::DestroyRange(U start, U end)
{
	for (; start != end; ++start)
	{
		start->~Type();
	}
}

template<class Type>
template <typename U>
std::enable_if_t<
	std::is_same_v<Type*, U> &&
	std::is_trivially_destructible_v<std::remove_pointer_t<U>>
>
inline DynamicArray<Type>::DestroyRange(U /*start*/, U /*end*/)
{
}

template<class Type>
inline void DynamicArray<Type>::MoveForward(uint32 startIndex, uint32 count)
{
	Assert(startIndex + count < arraySize);
	pointerType origLoc = GetData() + startIndex;
	pointerType destLoc = GetData() + startIndex + count;
	size_t memSize = (arraySize - startIndex) * sizeof(valueType);
	Memmove(destLoc, memSize, origLoc, memSize);
}

template<class Type>
inline void DynamicArray<Type>::MoveBack(uint32 startIndex, uint32 count)
{
	Assert(startIndex > 0);
	Assert(startIndex >= count);
	if (startIndex < arraySize)
	{
		pointerType origLoc = GetData() + startIndex;
		pointerType destLoc = GetData() + startIndex - count;
		size_t memSize = (arraySize - startIndex) * sizeof(valueType);
		Memmove(destLoc, memSize, origLoc, memSize);
	}
}

template<class Type>
inline void DynamicArray<Type>::AdjustSizeGeom(uint32 newSize)
{
	uint32 sizeAdjust = 3;
	if (newSize > arrayCapacity || arrayCapacity > 0)
	{
		sizeAdjust = static_cast<uint32>(newSize * 1.5f);
	}
	arrayCapacity += sizeAdjust;
	CreateAdjustedSpace();
}

template<class Type>
inline void DynamicArray<Type>::CreateAdjustedSpace()
{
	Type* tmpData = reinterpret_cast<Type*>(malloc(sizeof(Type) * arrayCapacity));
	Memcpy(tmpData, sizeof(Type) * arrayCapacity, data, arraySize * sizeof(Type));
	free(data);
	data = tmpData;
}

//////////////////////////////////////////////////////////////////////////
template<typename Elem>
inline uint32 GetHash(const DynamicArray<Elem>& arr)
{
	uint32 hash = 0;
	for (const auto& elem : arr)
	{
		HashCombine(hash, elem);
	}

	return hash;
}
