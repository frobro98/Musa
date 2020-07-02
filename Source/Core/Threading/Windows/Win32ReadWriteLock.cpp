
#include "Threading/ReadWriteLock.hpp"

ReadWriteLock::ReadWriteLock()
{
	::InitializeSRWLock(&rwLock);
}

void ReadWriteLock::LockRead()
{
	::AcquireSRWLockShared(&rwLock);
}

void ReadWriteLock::LockWrite()
{
	::AcquireSRWLockExclusive(&rwLock);
}

void ReadWriteLock::UnlockRead()
{
	::ReleaseSRWLockShared(&rwLock);
}

void ReadWriteLock::UnlockWrite()
{
	::ReleaseSRWLockExclusive(&rwLock);
}