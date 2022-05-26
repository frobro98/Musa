// Copyright 2020, Nathan Blane

#include "Debugging/DebugOutput.hpp"
#include "Platform/PlatformDefinitions.h"

constexpr u32 DebugBufferSize = 256;

namespace Debug
{
	// TODO - This needs to be in a Windows specific file
	void Print(const tchar* str)
	{
		OutputDebugString(str);
	}

	void Print(const fmt::memory_buffer& buf)
	{
		OutputDebugString(buf.data());
	}
}