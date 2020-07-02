#pragma once

#include "BasicTypes/Uncopyable.hpp"
#include "Threading/ScopedReadLock.hpp"
#include "Threading/ScopedWriteLock.hpp"
#include "Containers/DynamicArray.hpp"

template <class Elem>
class LockingQueue : private Uncopyable
{
public:
	LockingQueue();

private:

};
