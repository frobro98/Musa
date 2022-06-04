// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "CoreAPI.hpp"
WALL_WRN_PUSH
#include "fmt/format.h"
WALL_WRN_POP

namespace Debug
{
CORE_API void Print(const tchar* str);
CORE_API void Print(const fmt::memory_buffer& buf);

template <typename... Args>
CORE_TEMPLATE void Printf(const tchar* strFormat, Args... args)
{
	fmt::memory_buffer buf;
	fmt::format_to(buf.begin(), strFormat, args...);
	Print(buf);
}
}