#pragma once

#include "Platform/Platform.hpp"
#include "BasicTypes/Uncopyable.hpp"
#include "BasicTypes/Limits.hpp"

// TODO - This isn't the optimal way to actually define a primitive without inheritance...

constexpr

class Semaphore final : private Uncopyable
{
public:
	explicit Semaphore(i32 startingCount);
	explicit Semaphore(i32 startingCount, i32 maxCount);
	~Semaphore();

	void Wait(u32 msWait = U32Max);
	bool TryWait();
	u32 Signal();

private:
	void* semaphore;
};
