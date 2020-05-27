// Copyright 2020, Nathan Blane

#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>

#include "BasicTypes/Intrinsics.hpp"

struct ThreadSleepPrimitive
{
	std::mutex mut;
	std::condition_variable condVar;
};

void SetThreadName(std::thread& thread, const tchar* threadName);
void SetCurrentThreadName(const tchar*threadName);
u64 GetCurrentThreadID();

u32 GetNumberOfProcessors();
