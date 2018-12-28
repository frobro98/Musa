#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define GRAPHICS_API_VK 1

#if GRAPHICS_API_VK
#define VK_USE_PLATFORM_WIN32_KHR
#define VK_PLATFORM_SURFACE_EXTENSION VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#else
//#error DX12 implementation doesn't exist yet
#endif

#if _DEBUG 
#define DEBUG
#endif

#include "Win32InputDefinitions.h"