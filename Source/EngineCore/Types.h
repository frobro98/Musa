#pragma once

#include "Windows/WindowsTypes.h"

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
