#pragma once

#include "Containers/DynamicArray.hpp"

template <typename Type>
class Stack
{
public:
	using ValueType = Type;
	using ReferenceType = Type&;
	using PointerType = Type*;

public:
	Stack() = default;
	Stack(uint32 startStack);

	template<typename PushType>
	void Push(PushType&& item);
	Type Pop();
	Type& Peek();

	uint32 Size() const;
	bool IsEmpty() const;

private:
	DynamicArray<ValueType> stackSpace;
};

template<typename Type>
inline Stack<Type>::Stack(uint32 startStack)
{
	stackSpace.Reserve(startStack);
}

template<typename Type>
template<typename PushType>
inline void Stack<Type>::Push(PushType&& item)
{
	stackSpace.Add(std::forward<PushType>(item));
}

template<typename Type>
inline Type Stack<Type>::Pop()
{
	ValueType popped = stackSpace.Last();
	stackSpace.RemoveLast(popped);
	return popped;
}

template<typename Type>
inline Type& Stack<Type>::Peek()
{
	return stackSpace.Last();
}

template<typename Type>
inline uint32 Stack<Type>::Size() const
{
	return stackSpace.Size();
}

template<typename Type>
inline bool Stack<Type>::IsEmpty() const
{
	return stackSpace.IsEmpty();
}
