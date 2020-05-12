// Copyright 2020, Nathan Blane

#pragma once

#include "Types/ConcurrentTypes.hpp"
#include "Thread/JobSystem/Job.hpp"
#include "MemoryUtilities.h"
#include "Thread/ThreadingUtilities.hpp"

struct Job;

// Job System data structure probably
template <uint32 size>
class WorkStealingDeque
{
	static_assert(IsPowerOf2(size), "Size of Collection is restricted to powers of 2!");
public:
	WorkStealingDeque(uint64 owningThreadID);

	// Push and Pop can only be called from the queue's owning thread
	bool TryPush(Job& job);
	bool TryPop(Job*& job);
	bool TrySteal(Job*& job);

	void PrepareQueueForJobs();

private:
	static constexpr uint32 JobMask = size - 1;
	Job* jobs[size]{};
	atom32 topIndex, bottomIndex;
	const uint64 owningThreadID;
};

template<uint32 size>
inline WorkStealingDeque<size>::WorkStealingDeque(uint64 owningThreadID_)
	: owningThreadID(owningThreadID_)
{
	topIndex.store(0, std::memory_order_seq_cst);
	bottomIndex.store(0, std::memory_order_seq_cst);
}

template<uint32 size>
inline bool WorkStealingDeque<size>::TryPush(Job& job)
{
	Assert(GetCurrentThreadID() == owningThreadID);
	Assert(topIndex.load() <= bottomIndex.load());

	int32 bottom = bottomIndex.load(std::memory_order_acquire);
	int32 top = topIndex.load(std::memory_order_seq_cst);
	if((bottom - top) < size)
	{
		jobs[bottom & JobMask] = &job;
		bottomIndex.fetch_add(1, std::memory_order_release);
		Assert(topIndex.load() <= bottomIndex.load());
		return true;
	}

	Assert(topIndex.load() <= bottomIndex.load());
	return false;
}

template<uint32 size>
inline bool WorkStealingDeque<size>::TrySteal(Job*& job)
{
	int32 top = topIndex.load(std::memory_order_seq_cst);
	int32 bottom = bottomIndex.load(std::memory_order_seq_cst);

	if (top < bottom)
	{
		if (topIndex.compare_exchange_strong(top, top + 1, std::memory_order_seq_cst, std::memory_order_seq_cst))
		{
			job = jobs[top & JobMask];
			Assert(top + 1 <= bottom);
			return true;
		}
	}

	return false;
}

template<uint32 size>
inline bool WorkStealingDeque<size>::TryPop(Job*& job)
{
	Assert(GetCurrentThreadID() == owningThreadID);

	int32 popLoc = bottomIndex.load(std::memory_order_acquire) - 1;
	bottomIndex.store(popLoc, std::memory_order_release);

	int32 top = topIndex.load(std::memory_order_seq_cst);
	if (top <= popLoc)
	{
		if (top != popLoc)
		{
			job = jobs[popLoc & JobMask];
			Assert(top < popLoc);
			return true;
		}

		// Last item in queue
		if (topIndex.compare_exchange_strong(top, top + 1, std::memory_order_seq_cst, std::memory_order_seq_cst))
		{
			job = jobs[popLoc & JobMask];
			bottomIndex.store(top + 1, std::memory_order_seq_cst);
			Assert(top + 1 == popLoc + 1);
			return true;
		}
	}

	bottomIndex.store(top, std::memory_order_seq_cst);
	Assert(topIndex.load() <= bottomIndex.load());
	return false;
}

template<uint32 size>
inline void WorkStealingDeque<size>::PrepareQueueForJobs()
{
	// Assert that queue is empty
// 	int32 top = topIndex.load();
// 	int32 bot = bottomIndex.load();
// 	Assert(top == bot);

	topIndex.store(0, std::memory_order_seq_cst);
	bottomIndex.store(0, std::memory_order_seq_cst);
}
