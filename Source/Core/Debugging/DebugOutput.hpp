// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "CoreAPI.hpp"
#include "fmt/format.h"

#include "Platform/Platform.hpp"

namespace Debug
{
CORE_API void Print(const tchar* str);

template <typename... Args>
CORE_TEMPLATE void Printf(const tchar* strFormat, Args... args)
{
	fmt::memory_buffer buf;
	fmt::format_to(buf, strFormat, args...);
	OutputDebugString(buf.data());
}
}