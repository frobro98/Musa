#include "Assertion.h"
 
void Debug::AssertionFailed(const tchar * expr, const tchar * file, int32 line, const tchar* desc)
{
	UNUSED(expr, file, line, desc);
	DebugBreak();
}

void Debug::AssertionFailedWithDescription(const tchar * expr, const tchar * file, int32 line, const tchar * desc, ...)
{
	UNUSED(expr, file, line, desc);
	DebugBreak();
}
