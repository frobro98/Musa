

#pragma once

#include "String/String.h"
WALL_WRN_PUSH
#include "fmt/format.h"
WALL_WRN_POP

// TODO - Remove this and make standalone function helper
template<typename... StrArgs>
static String Format(const tchar* formatStr, StrArgs... args)
{
	fmt::memory_buffer buf;
	fmt::format_to(std::back_inserter(buf), formatStr, args...);
	return String(buf.data(), (u32)buf.size());
}
