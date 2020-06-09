#include "Win32SyncEvent.hpp"
#include "Debugging/Assertion.hpp"

Win32SyncEvent::~Win32SyncEvent()
{
	if (eventHandle != nullptr)
	{
		CloseHandle(eventHandle);
	}
}

void Win32SyncEvent::Create(bool manualReset)
{
	isManualReset = manualReset;
	CreateEvent(nullptr, isManualReset, 0, nullptr);
}

bool Win32SyncEvent::Wait(u32 waitTime)
{
	Assert(eventHandle);
	BOOL status = WaitForSingleObject(eventHandle, waitTime);
	return status == WAIT_OBJECT_0;
}

void Win32SyncEvent::Set()
{
	Assert(eventHandle);
	::SetEvent(eventHandle);
}

bool Win32SyncEvent::Reset()
{
	Assert(eventHandle);
	bool hasReset = ::ResetEvent(eventHandle);
	isSet = !hasReset;
	return hasReset;
}

bool Win32SyncEvent::IsManualReset() const
{
	return isManualReset;
}

bool Win32SyncEvent::IsSet() const
{
	return isSet;
}

bool Win32SyncEvent::IsValid() const
{
	return eventHandle != nullptr;
}
