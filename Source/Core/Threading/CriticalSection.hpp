#pragma once

#include "Platform/Platform.hpp"
#include "BasicTypes/Uncopyable.hpp"

// TODO - SRWLock is more optimal than CRITICAL_SECTION. Change this to use that
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
