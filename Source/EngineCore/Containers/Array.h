#pragma once

#include "Platform.h"
#include "Assertion.h"
#include "MemoryUtilities.h"
#include "Serialization/SerializeBase.hpp"
#include "Serialization/DeserializeBase.hpp"

template<class Type>
class Array
{
	using valueType = Type;
	using pointerType = Type*;
	using referenceType = Type&;
	// Required because other template types are different Array types
	template<typename OtherType>
	friend class Array;

public:
	Array()
		: data(nullptr),
		arraySize(0),
		arrayCapacity(0)
	{
	}

	explicit Array(uint32 initialSize)
		:arraySize(initialSize),
		arrayCapacity(initialSize)
	{
		data = reinterpret_cast<Type*>(malloc(sizeof(Type) * initialSize));
		Construct(0, arraySize);
	}

	template <size_t N>
	Array(const Type(&arr)[N])
		: arraySize(N),
		arrayCapacity(N)
	{
		CopyData(arr, N);
	}

	Array(const Type* arr, uint32 elementCount)
		: arraySize(elementCount),
		arrayCapacity(elementCount)
	{
		CopyData(arr, elementCount);
	}

	Array(std::initializer_list<Type> list)
		: arraySize(0),
		arrayCapacity(0)
	{
		Reserve(list.size());
		const Type* beginning = list.begin();
		const Type* ending = list.end();
		for (; beginning != ending; ++beginning)
		{
			Add(*beginning);
		}
	}

	~Array()
	{
		free(data);
		arrayCapacity = 0;
		arraySize = 0;
	}

	Array(const Array& otherArr)
	{
		arrayCapacity = otherArr.arraySize;
		CopyData(otherArr.data, otherArr.arraySize);
	}

	Array(Array&& otherArr) noexcept
	{
		DestroyAll();
		free(data);

		data = otherArr.data;
		arraySize = otherArr.arraySize;
		arrayCapacity = otherArr.arrayCapacity;

		otherArr.data = nullptr;
		otherArr.arraySize = 0;
		otherArr.arrayCapacity = 0;
	}

	template <class OtherType>
	Array(const Array<OtherType>& otherArr)
	{
		CopyData(otherArr.GetData(), otherArr.arraySize);
		arrayCapacity = otherArr.arrayCapacity;
	}

	Array& operator=(const Array& otherArr)
	{
		if (this != &otherArr)
		{
			CopyData(otherArr.GetData(), otherArr.arraySize);
			arrayCapacity = otherArr.arrayCapacity;
		}

		return *this;
	}

	Array& operator=(Array&& otherArr) noexcept
	{
		if (this != &otherArr)
		{
			DestroyAll();
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

private:
	template <class OtherType>
	void CopyData(OtherType* otherData, uint32 dataNum)
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

public:

	template <typename AddType>
	uint32 Add(AddType&& newElement)
	{
		uint32 newSize = arraySize + 1;
		if (newSize > arrayCapacity)
		{
			AdjustSizeGeom(newSize);
		}
		
		return Emplace(std::forward<AddType>(newElement));
	}

	// Creates an empty element in the array. Element isn't initialized in any way
	uint32 AddEmpty(uint32 emptyElements = 1)
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


	uint32 AddDefault(uint32 emptyElements = 1)
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

	template <typename InsertType>
	void Insert(InsertType&& elem, uint32 index)
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

	bool TryRemoveElement(const valueType& elem)
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

	void Remove(uint32 index, uint32 count = 1)
	{
		Assert(index < arraySize);
		Destroy(index, index+count);
		MoveBack(index+count, count);
		arraySize -= count;
	}

	void RemoveAll(const valueType& elem)
	{
		for (uint32 i = 0; i < arraySize; ++i)
		{
			if (GetData()[i] == elem)
			{
				Remove(i);
			}
		}
	}

	bool TryFindFirstIndex(const valueType& obj, uint32& foundIndex)
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

	bool TryFindLastIndex(const valueType& obj, uint32& foundIndex)
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

	bool TryFindFirst(const valueType& obj, valueType& foundVal)
	{
		uint32 i;
		bool found = TryFindFirstIndex(obj, i);
		if (found)
		{
			foundVal = data[i];
		}

		return found;
	}

	bool TryFindLast(const valueType& obj, valueType& foundVal)
	{
		uint32 i;
		bool found = TryFindLastIndex(obj, i);
		if (found)
		{
			foundVal = data[i];
		}

		return found;
	}

public:
	void Reserve(uint32 reserveCapacity)
	{
		if (arrayCapacity < reserveCapacity)
		{
			arrayCapacity = reserveCapacity;
			CreateAdjustedSpace();
		}
	}

	void Resize(uint32 newSize)
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

	void Clear()
	{
		if (arraySize > 0)
		{
			Destroy(0, arraySize);
			arraySize = 0;
		}
	}

	bool IsEmpty() const
	{
		return arraySize == 0;
	}

private:
	void Construct(uint32 index)
	{
		Assert(index >= 0 && index < arraySize);
		ConstructRange(data + index, data + (index + 1));
	}

	void Construct(uint32 startIndex, uint32 endIndex)
	{
		Assert(startIndex >= 0 && startIndex < arraySize);
		Assert(endIndex >= startIndex && endIndex <= arraySize);
		ConstructRange(data + startIndex, data + endIndex);
	}

	template <typename U>
	std::enable_if_t<
		std::is_same_v<pointerType, U> &&
		std::is_scalar_v<std::remove_pointer_t<U>>
	>
		ConstructRange(U start, U end)
	{
		uint8* const beginning = reinterpret_cast<uint8*>(start);
		uint8* const ending = reinterpret_cast<uint8*>(end);
		Memset(beginning, 0, static_cast<size_t>(ending - beginning));
	}

	template <typename U>
	std::enable_if_t<
		std::is_same_v<pointerType, U> &&
		!std::is_scalar_v<std::remove_pointer_t<U>>
	>
		ConstructRange(U start, U end)
	{
		for (; start != end; ++start)
		{
			new(static_cast<void*>(start)) Type();
		}
	}

	void Destroy(uint32 index)
	{
		Assert(index >= 0 && index < arraySize);
		DestroyRange(data + index, data + (index+1));
	}

	void Destroy(uint32 startIndex, uint32 endIndex)
	{
		Assert(startIndex >= 0 && startIndex < arraySize);
		Assert(endIndex >= startIndex && endIndex <= arraySize);
		DestroyRange(data + startIndex, data + endIndex);
	}

	void DestroyAll()
	{
		if (arraySize > 0)
		{
			DestroyRange(data, data + arraySize);
		}
	}

	template <typename U>
	std::enable_if_t<
		std::is_same_v<pointerType, U> && 
		!std::is_trivially_destructible_v<std::remove_pointer_t<U>>
	> 
		DestroyRange(U start, U end)
	{
		for (; start != end; ++start)
		{
			start->~Type();
		}
	}

	template <typename U>
	std::enable_if_t<
		std::is_same_v<pointerType, U> && 
		std::is_trivially_destructible_v<std::remove_pointer_t<U>>
	> 
		DestroyRange(U /*start*/, U /*end*/)
	{
	}

	void MoveForward(uint32 startIndex, uint32 count = 1)
	{
		Assert(startIndex + count < arraySize);
		pointerType origLoc = GetData() + startIndex;
		pointerType destLoc = GetData() + startIndex + count;
		size_t memSize = (arraySize - startIndex) * sizeof(valueType);
		Memmove(destLoc, memSize, origLoc, memSize);
	}

	void MoveBack(uint32 startIndex, uint32 count = 1)
	{
		Assert(startIndex > 0);
		Assert(startIndex >= count);
		Assert(startIndex <= arraySize);
		pointerType origLoc = GetData() + startIndex;
		pointerType destLoc = GetData() + startIndex - count;
		size_t memSize = (arraySize - startIndex) * sizeof(valueType);
		Memmove(destLoc, memSize, origLoc, memSize);
	}

public:
	
	Type* GetData()
	{
		Assert(arrayCapacity > 0);
		return data;
	}

	const Type* GetData() const
	{
		return data;
	}

	template<class... Args>
	uint32 Emplace(Args... args)
	{
		uint32 index = AddEmpty();
		new(GetData() + index) Type(std::forward<Args>(args)...);
		return index;
	}

	uint32 Size() const
	{
		return arraySize;
	}

	uint32 Capacity() const
	{
		return arrayCapacity;
	}

	bool Contains(const Type& obj) const
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

	// Accessors
	Type& operator[](uint32 index)
	{
		Assert(index < arraySize);
		return GetData()[index];
	}

	const Type& operator[](uint32 index) const
	{
		Assert(index < arraySize);
		return GetData()[index];
	}

	const Type& First()
	{
		Assert(arraySize > 0);
		return data[0];
	}

	const Type& Last()
	{
		Assert(arraySize > 0);
		return data[arraySize - 1];
	}

private:

	void AdjustSizeGeom(uint32 newSize)
	{
		uint32 sizeAdjust = 3;
		if (newSize > arrayCapacity || arrayCapacity > 0)
		{
			sizeAdjust = static_cast<uint32>(newSize * 1.5f);
		}
		arrayCapacity += sizeAdjust;
		CreateAdjustedSpace();
	}

	void CreateAdjustedSpace()
	{
		Type* tmpData = reinterpret_cast<Type*>(malloc(sizeof(Type) * arrayCapacity));
		Memcpy(tmpData, sizeof(Type) * arrayCapacity, data, arraySize * sizeof(Type));
		free(data);
		data = tmpData;
	}

public:
	class Iterator final
	{
	public:
		Iterator(Array<Type>& arr_)
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
		bool operator!=(Iterator& other)
		{
			return arr != other.arr;
		}

		Type& operator*()
		{
			return (*arr)[currentIndex];
		}

	private:
		Array<Type>* arr;
		uint32 currentIndex = 0;
	};

	class ConstIterator
	{
	public:
		ConstIterator(const Array<Type>& arr_)
			: arr(&arr_)
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
		bool operator!=(ConstIterator& other)
		{
			return arr != other.arr;
		}

		const Type& operator*() const
		{
			return (*arr)[currentIndex];
		}

	private:
		const Array<Type>* arr;
		uint32 currentIndex = 0;
	};

private:
	friend Iterator begin(Array& arr) { return Iterator(arr); }
	friend ConstIterator begin(const Array& arr) { return ConstIterator(arr); }
	friend Iterator end(Array& /*arr*/) { return Iterator(nullptr); }
	friend ConstIterator end(const Array& /*arr*/) { return ConstIterator(nullptr); }

	friend void Serialize(SerializeBase& ser, const Array& arr)
	{
		Serialize(ser, arr.arraySize);

		for (uint32 i = 0; i < arr.arraySize; ++i)
		{
			Serialize(ser, arr.data[i]);
		}
	}

	friend void Deserialize(DeserializeBase& ser, Array& arr)
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
	Type* data = nullptr;
	uint32 arraySize = 0;
	uint32 arrayCapacity = 0;
};

template<typename Elem>
inline uint32 GetHash(const Array<Elem>& arr)
{
	uint32 hash = 0;
	for (const auto& elem : arr)
	{
		hash |= GetHash(elem);
	}

	return hash;
}
