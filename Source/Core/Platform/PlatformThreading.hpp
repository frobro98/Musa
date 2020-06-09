#pragma once

class NativeThread;
class ISyncEvent;

namespace PlatformThreading
{
NativeThread* CreateThread();
ISyncEvent* CreateSyncEvent(bool isManualReset);
}
