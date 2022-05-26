#pragma once

// TODO - Remove this somehow
#include "Platform/PlatformDefinitions.h"

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
