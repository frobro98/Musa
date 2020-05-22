// Copyright 2020, Nathan Blane

#include "Debugging/DebugOutput.hpp"
#include "Platform/Platform.hpp"

constexpr uint32 DebugBufferSize = 256;

namespace Debug
{
	// TODO - This needs to be in a Windows specific file
	void Print(const tchar* str)
	{
		OutputDebugString(str);
	}
}