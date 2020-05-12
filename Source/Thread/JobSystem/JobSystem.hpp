// Copyright 2020, Nathan Blane

#pragma once

#include "Thread/JobSystem/Job.hpp"
#include "Containers/DynamicArray.hpp"

class JobThread;

class JobSystem
{
public:
	void Initialize();
	void StartSystem();
	void Deinitialize();

	void SetJobQueueForIndex(JobQueue& q, uint32 index);

	JobQueue& GetRandomQueue(uint32 index) const;

	Job* CreateEmptyJob() const;
	Job* CreateJob(JobTask&& func) const;
	Job* CreateJobChild(Job& parent, JobTask&& func) const;

	void PushJob(Job& job);
	void WaitOnJob(const Job& job);

	void BeginFrame();
	void EndOfFrame();

private:
	ThreadSleepPrimitive systemSleepPrim;
	DynamicArray<JobThread*> jobThreads;
	DynamicArray<JobQueue*> jobQueues;
};

JobSystem& GetJobSystem();