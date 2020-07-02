// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "CoreAPI.hpp"

// NOTE - This is implemented in a similar way to how std::deque would be implemented...
// If it's performance necessary, you can't go with the DynamicArray alone

constexpr u32 QueueNodeSize = 512;

template <typename T>
class CORE_TEMPLATE Queue
{
public:
	Queue() = default;
	~Queue();

	template <class PushType>
	void Push(PushType&& elem);
	T& Peek();
	T Pop();

	bool IsEmpty() const;
	u32 Size() const;

private:
	// Contains the math that actually figures out how many elements
	constexpr forceinline u32 GetCountPerBlock()
	{
		return sizeof(T) <= QueueNodeSize ?
			QueueNodeSize / sizeof(T) : 1;
	}

	forceinline void ExpandBlocks()
	{
		u32 newBlockCount = blockCount > 0 ? blockCount * 2 : 1;
		queueBlocks = (T**)Memory::Realloc(queueBlocks, sizeof(T**) * newBlockCount);
		blockCount = newBlockCount;
	}

	forceinline T* GetAssociatedBlock(u32 off)
	{
		constexpr u32 blockElems = GetCountPerBlock();

	}
private:
	T** queueBlocks = nullptr;
	u32 offset = 0;
	u32 size = 0;
	u32 blockCount = 0;
};

template<typename T>
forceinline Queue<T>::~Queue()
{
}

template<typename T>
template<class PushType>
forceinline void Queue<T>::Push(PushType&& elem)
{
}

template<typename T>
forceinline T& Queue<T>::Peek()
{
	// TODO: insert return statement here
}

template<typename T>
forceinline T Queue<T>::Pop()
{
	return T();
}

template<typename T>
forceinline bool Queue<T>::IsEmpty() const
{
	return false;
}

template<typename T>
forceinline u32 Queue<T>::Size() const
{
	return u32();
}

