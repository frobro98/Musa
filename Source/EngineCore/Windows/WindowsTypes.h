#pragma once

#include "Windows/WindowsDefinitions.h"

// TODO - Figure out if these type definitions are actually needed for windows or linux or macOS
// Windows unsigned definition
using uint64 = DWORD64;
using uint64 = UINT64;

using uint32 = UINT32;

using uint16 = WORD;
using uint16 = UINT16;

using uint8 = BYTE;
using uint8 = UINT8;

// Windows signed definitions
using int64 = INT64;
using int32 = INT32;
using int16 = INT16;
using int8 = INT8;