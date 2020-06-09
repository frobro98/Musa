
#include "Platform/PlatformThreading.hpp"
#include "Threading/Windows/Win32NativeThread.hpp"
#include "Threading/Windows/Win32SyncEvent.hpp"

namespace PlatformThreading
{
NativeThread* CreateThread()
{
	return new Win32NativeThread;
}

ISyncEvent* CreateSyncEvent(bool isManualReset)
{
	ISyncEvent* event = new Win32SyncEvent;
	event->Create(isManualReset);
	if (!event->IsValid())
	{
		delete event;
		event = nullptr;
	}
	return event;
}
}