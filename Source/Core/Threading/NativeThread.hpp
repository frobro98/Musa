#pragma once

#include "String/String.h"

class IThreadBody;

enum class ThreadPriority
{
	High,
	Normal,
	Low
};

class NativeThread
{
public:
	virtual ~NativeThread() {}

	virtual void StartWithBody(const tchar* threadName, ThreadPriority threadPriority, IThreadBody& execution) = 0;
	virtual void SetPriority(ThreadPriority priority) = 0;
	virtual void ResumeThread() = 0;
	virtual void SuspendThread() = 0;
	virtual void WaitStop() = 0;
	virtual void WaitJoin() = 0;

	forceinline u32 GetThreadID() const
	{
		return threadId;
	}

	forceinline const String& GetThreadName() const
	{
		return threadName;
	}

protected:
	String threadName;
	IThreadBody* executionBody = nullptr;
	ThreadPriority threadPriority;
	u32 threadId = 0;
};
