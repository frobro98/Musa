#include "RefPtr.hpp"

void Counter::Increment()
{
	++count;
}

void Counter::Decrement()
{
	--count;
}

ThreadSafeCounter::ThreadSafeCounter()
{
	count.store(1, std::memory_order_seq_cst);
}

ThreadSafeCounter::ThreadSafeCounter(const ThreadSafeCounter& other)
{
	uint32 c = other.count.load(std::memory_order_acquire);
	count.store(c, std::memory_order_release);
}

ThreadSafeCounter::ThreadSafeCounter(ThreadSafeCounter&& other) noexcept
{
	uint32 c = other.count.load(std::memory_order_acquire);
	count.store(c, std::memory_order_release);
}

ThreadSafeCounter& ThreadSafeCounter::operator=(const ThreadSafeCounter& other)
{
	if (this != &other)
	{
		uint32 c = other.count.load(std::memory_order_acquire);
		count.store(c, std::memory_order_release);
	}
	return *this;
}

ThreadSafeCounter& ThreadSafeCounter::operator=(ThreadSafeCounter&& other) noexcept
{
	if (this != &other)
	{
		uint32 c = other.count.load(std::memory_order_acquire);
		count.store(c, std::memory_order_release);
	}
	return *this;
}

void ThreadSafeCounter::Increment()
{
	count.fetch_add(1);
}

void ThreadSafeCounter::Decrement()
{
	count.fetch_sub(1);
}
