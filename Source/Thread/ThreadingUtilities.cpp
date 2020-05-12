// Copyright 2020, Nathan Blane

#include "ThreadingUtilities.hpp"
#include "Assertion.h"
#include "Containers/ConcurrentQueue.hpp"

namespace
{
constexpr DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
	DWORD dwType; // Must be 0x1000.
	LPCSTR szName; // Pointer to name (in user addr space).
	DWORD dwThreadID; // Thread ID (-1=caller thread).
	DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)
}

void SetThreadName(std::thread& thread, const tchar* threadName)
{
	DWORD threadId = ::GetThreadId(static_cast<HANDLE>(thread.native_handle()));

	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = threadName;
	info.dwThreadID = threadId;
	info.dwFlags = 0;

	__try
	{
		RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}
}

void SetCurrentThreadName(const tchar* threadName)
{
	DWORD threadId = ::GetThreadId(GetCurrentThread());

	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = threadName;
	info.dwThreadID = threadId;
	info.dwFlags = 0;

	__try
	{
		RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}
}

uint64 GetCurrentThreadID()
{
	return std::hash<std::thread::id>()(std::this_thread::get_id());
}

uint32 GetNumberOfProcessors()
{
	uint32 cores = std::thread::hardware_concurrency();
//	Assert(cores > 0);
	return cores;
}
