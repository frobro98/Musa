#pragma once

#include "Platform/Platform.hpp"
#include "BasicTypes/Uncopyable.hpp"

class CriticalSection final : private Uncopyable
{
public:
	CriticalSection();
	~CriticalSection();

	void Lock();
	bool TryLock();
	void Unlock();

private:
	CRITICAL_SECTION critSection;
};
