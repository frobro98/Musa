#pragma once

#define WALL_WRN_PUSH				  \
	__pragma(warning( push ))		  \
	__pragma(warning(disable : 4820)) \
	__pragma(warning(disable : 4365)) \
	__pragma(warning(disable : 4005)) \
	__pragma(warning(disable : 4571)) \
	__pragma(warning(disable : 4625)) \
	__pragma(warning(disable : 4626)) \
    __pragma(warning(disable : 4774)) \
	__pragma(warning(disable : 4623)) \
    __pragma(warning(disable : 5027)) \
	__pragma(warning(disable : 4464)) \
	__pragma(warning(disable : 4061)) \
    __pragma(warning(disable : 5026)) \
	__pragma(warning(disable : 5039)) \
	__pragma(warning(disable : 4820)) 

#define WALL_WRN_POP \
	__pragma(warning( pop ))

WALL_WRN_PUSH
#include <cstdio>
#include <cstdlib>

#include <new>
#include <memory>
#include <algorithm>
#include <functional>
WALL_WRN_POP

#ifdef _WIN32
#include "Windows/WindowsDefinitions.h"
#endif

#define UNUSED(...)						\
	__pragma(warning(push))				\
	__pragma(warning(disable : 4548))	\
	(__VA_ARGS__)						\
	__pragma(warning(pop))
	
#define ArraySize(arr) ( ( (sizeof(arr)) / (sizeof(arr[0])) ) )

#include "Types/Intrinsics.hpp"
