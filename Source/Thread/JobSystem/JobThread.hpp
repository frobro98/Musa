#pragma once

#include <thread>
#include <condition_variable>

#include "Types/ConcurrentTypes.hpp"
#include "Thread/JobSystem/Job.hpp"
#include "Thread/ThreadingUtilities.hpp"

class JobSystem;

class JobThread
{
public:
	JobThread(JobSystem& jobSys, uint32 index, ThreadSleepPrimitive& systemSleepPrim);
	~JobThread();

	JobThread(const JobThread&) = delete;
	JobThread& operator=(const JobThread&) = delete;

	void Start();
	void WaitStop();

	void WakeUp();

	JobQueue& GetWorkQueue() const;

private:
	void ThreadLoop();
	bool FindAndExecuteJob();

private:

	// TODO - Should probably use native threads...
	std::thread jobThread;
	ThreadSleepPrimitive jobNotify;
	ThreadSleepPrimitive& systemNotifier;
	JobSystem& jobSystem;
	JobQueue* jobQueue = nullptr;
	uflag threadRunning = ATOMIC_FLAG_INIT;
	uint32 threadIndex;
};
