#pragma once

#include "Platform/Platform.hpp"
#include "BasicTypes/Uncopyable.hpp"

// TODO - This isn't the optimal way to actually define a primitive without inheritance...
class Semaphore final : private Uncopyable
{
public:
	Semaphore(u32 startingCount, u32 maxCount);
	~Semaphore();

	void Wait(u32 msWait);
	bool TryWait();
	u32 Signal();

private:
	void* semaphore;
};
