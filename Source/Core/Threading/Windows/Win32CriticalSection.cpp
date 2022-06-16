
#include "Threading/CriticalSection.hpp"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

CriticalSection::CriticalSection()
{
	::InitializeCriticalSectionAndSpinCount(critSection, 0x00000400);
}

CriticalSection::~CriticalSection()
{
	::DeleteCriticalSection(critSection);
}

void CriticalSection::Lock()
{
	::EnterCriticalSection(critSection);
}

bool CriticalSection::TryLock()
{
	return ::TryEnterCriticalSection(critSection);
}

void CriticalSection::Unlock()
{
	::LeaveCriticalSection(critSection);
}
