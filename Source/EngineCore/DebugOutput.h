#pragma once

#include "PlatformDefinitions.h"
#include "fmt/format.h"

namespace Debug
{
	void Print(const tchar* str);

	template <typename... Args>
	void Printf(const tchar* strFormat, Args... args)
	{
		fmt::memory_buffer buf;
		fmt::format_to(buf, strFormat, args...);
		OutputDebugString(buf.data());
	}
}