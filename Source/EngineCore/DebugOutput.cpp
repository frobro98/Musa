#include "DebugOutput.h"
#include "String/CStringUtilities.hpp"

constexpr uint32 DebugBufferSize = 256;

namespace Debug
{
	// TODO - This needs to be in a Windows specific file
	void Print(const tchar* str)
	{
		OutputDebugString(str);
	}

	void Printf(const tchar* strFormat, ...)
	{
		tchar debugBuffer[DebugBufferSize];
		VA_ARGS_PARSE(strFormat, debugBuffer, DebugBufferSize);
		OutputDebugString(debugBuffer);
	}
}