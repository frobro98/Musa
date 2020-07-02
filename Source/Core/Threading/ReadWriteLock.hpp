#pragma once

#include "Platform/Platform.hpp"

class ReadWriteLock
{
public:
	ReadWriteLock();

	void LockRead();
	void LockWrite();
	void UnlockRead();
	void UnlockWrite();

private:
	SRWLOCK rwLock;
};
