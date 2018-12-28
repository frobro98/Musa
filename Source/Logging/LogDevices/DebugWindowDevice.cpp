#include "DebugWindowDevice.h"
#include "DebugOutput.h"

void DebugWindowDevice::Log(const tchar * logStr)
{
	Debug::Print(logStr);
}
