// Copyright 2020, Nathan Blane

#pragma once

#ifndef STRICT
#define STRICT
#endif 

#define WINAPI __stdcall

// Global Win32 defs
struct HINSTANCE__;
struct HWND__;
struct _RTL_SRWLOCK;
struct _RTL_CRITICAL_SECTION;
union _LARGE_INTEGER;

namespace Win32
{
using HANDLE = void*;
using HINSTANCE = HINSTANCE__*;

using DWORD = unsigned long;
using LPDWORD = DWORD*;
using LPVOID = void*;
using LONG_PTR = i64;

using LPCRITICAL_SECTION = _RTL_CRITICAL_SECTION*;
using RTL_SRWLOCK = _RTL_SRWLOCK;
using PRTL_SRWLOCK = _RTL_SRWLOCK*;

struct SRWLOCK
{
	void* p;
};

using LPLARGE_INTEGER = _LARGE_INTEGER*;
}
