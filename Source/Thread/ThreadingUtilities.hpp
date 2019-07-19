#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>

#include "EngineCore/Types/Intrinsics.hpp"

struct ThreadSleepPrimitive
{
	std::mutex mut;
	std::condition_variable condVar;
};

void SetThreadName(std::thread& thread, const tchar* threadName);
void SetCurrentThreadName(const tchar*threadName);
uint64 GetCurrentThreadID();

uint32 GetNumberOfProcessors();
