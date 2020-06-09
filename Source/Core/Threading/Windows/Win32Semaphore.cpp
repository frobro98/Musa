
#include "Threading/Semaphore.hpp"
#include "Debugging/Assertion.hpp"

static bool WaitOnSemaphore(HANDLE semaphore, u32 msWait)
{
	const u32 waitResult = ::WaitForSingleObject(semaphore, msWait);
	switch (waitResult)
	{
		case WAIT_OBJECT_0: return true;
		case WAIT_TIMEOUT: return false;
		case WAIT_ABANDONED: Assert(false); return false;
		case WAIT_FAILED: Assert(false); return false;
		default:
			Assert(false);
			return false;
	}
}

Semaphore::Semaphore(u32 startingCount, u32 maxCount)
	: semaphore((void*)::CreateSemaphore(nullptr, (i32)startingCount, (i32)maxCount, nullptr))
{
}

Semaphore::~Semaphore()
{
	CloseHandle((HANDLE)semaphore);
}

void Semaphore::Wait(u32 msWait)
{
	WaitOnSemaphore((HANDLE)semaphore, msWait);
}

bool Semaphore::TryWait()
{
	return WaitOnSemaphore((HANDLE)semaphore, 0);
}

u32 Semaphore::Signal()
{
	u32 prevSignalCount;
	::ReleaseSemaphore((HANDLE)semaphore, 1, (LPLONG)&prevSignalCount);
	return prevSignalCount;
}