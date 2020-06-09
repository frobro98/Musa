
#include "Threading/CriticalSection.hpp"

CriticalSection::CriticalSection()
{
	::InitializeCriticalSectionAndSpinCount(&critSection, 0x00000400);
}

CriticalSection::~CriticalSection()
{
	::DeleteCriticalSection(&critSection);
}

void CriticalSection::Lock()
{
	::EnterCriticalSection(&critSection);
}

bool CriticalSection::TryLock()
{
	return ::TryEnterCriticalSection(&critSection);
}

void CriticalSection::Unlock()
{
	::LeaveCriticalSection(&critSection);
}
