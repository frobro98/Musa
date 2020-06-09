#pragma once

#include "Platform/Platform.hpp"
#include "Threading/NativeThread.hpp"

class ISyncEvent;

class Win32NativeThread final : public NativeThread
{
public:
	Win32NativeThread() = default;
	~Win32NativeThread();

	virtual void StartWithBody(const tchar* threadName, ThreadPriority threadPriority, IThreadBody& execution) override;
	virtual void WaitStop() override;
	virtual void WaitJoin() override;
	virtual void SetPriority(ThreadPriority priority) override;
	virtual void ResumeThread() override;
	virtual void SuspendThread() override;

private:
	static DWORD ThreadProc(LPVOID parameter);
	u32 StartRun();

private:
	String threadName;
	HANDLE threadHandle = nullptr;
	ISyncEvent* syncEvent = nullptr;
	bool running = false;
};

