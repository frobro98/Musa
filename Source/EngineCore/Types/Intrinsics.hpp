#pragma once

#include <cstdint>

#include "Windows/WindowsTypes.h"

using uint64 = uint64_t;
using uint32 = uint32_t;
using uint16 = uint16_t;
using uint8 = uint8_t;

using int64 = int64_t;
using int32 = int32_t;
using int16 = int16_t;
using int8 = int8_t;

using unichar =  wchar_t;
using ansichar = char;

#ifdef UNICODE
using tchar = unichar;
#define text(x) L##x
#else
using tchar =  ansichar;
#define text(x) x
#endif

using float32 = float;
using float64 = double;

using bool32 = uint32;
