#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define VK_USE_PLATFORM_WIN32_KHR
#define VK_PLATFORM_SURFACE_EXTENSION VK_KHR_WIN32_SURFACE_EXTENSION_NAME

#include "Win32InputDefinitions.h"