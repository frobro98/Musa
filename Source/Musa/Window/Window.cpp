// Copyright 2020, Nathan Blane

#include "EngineCore/Platform.h"
#include "Assertion.h"
#include "Window.h"
#include "Input/InputDefinitions.hpp"
#include "Input/Internal/InputInternal.hpp"


Window::Window(HINSTANCE instance, WindowInputHandler& inputHandler_, uint32 xPos, uint32 yPos, uint32 w, uint32 h)
	: window(nullptr),
	inputHandler(inputHandler_),
	position(xPos, yPos),
	width(w),
	height(h),
	isShown(false)
{


	DWORD style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
	DWORD exStyle = 0;
	window = CreateWindowEx(exStyle,
		// TODO - Make the window class name some shared variable that the WindowsApp controls and everything else knows about or something...
		TEXT("MusaEngine"), TEXT("Musa Engine Window"),
		style,
		xPos, yPos,
		width, height,
		nullptr, nullptr, instance, nullptr);

	if (!window)
	{
		Assert(false);
	}

	SetWindowLongPtr(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&inputHandler));


	RECT windowRect;
	GetWindowRect(window, &windowRect);

	Show();
//	ShowCursor(FALSE);

// 	int32 originX = windowRect.left;
// 	int32 originY = windowRect.top;
// 	RECT cursorRect = {};
// 	cursorRect.left = originX;
// 	cursorRect.top = originY;
// 	cursorRect.right = width;
// 	cursorRect.bottom = height;
// 	ClipCursor(&cursorRect);
// 
// 	int32 centerX = originX + (width / 2);
// 	int32 centerY = originY + (height / 2);
// 	SetCursorPos(centerX, centerY);

	isActive = true;

	// 	if (newWindowMode == WindowMode::Fullscreen)
	// 	{
	// 		SetWindowMode(windowMode);
	// 	}
	// 	else
	// 	{
	// 		windowMode = WindowMode::Windowed;
	// 	}
}

Window::~Window()
{
	Assert(window != nullptr);
	DestroyWindow(window);
}

void Window::Close()
{
	if (isActive)
	{
		isActive = false;
	}
}

void Window::Resize(uint32 w, uint32 h)
{
	UNUSED(w, h);
}

void Window::Show()
{
	if (!isShown)
	{
		ShowWindow(window, SWP_SHOWWINDOW);
		isShown = true;
	}
}

void Window::Hide()
{
	if (isShown)
	{
		ShowWindow(window, SW_HIDE);
		isShown = false;
	}
}

void Window::SetWindowMode(WindowMode newWindowMode)
{
	if (windowMode != newWindowMode)
	{
		windowMode = newWindowMode;

		LONG currentStyle = GetWindowLong(window, GWL_STYLE);
		DWORD windowedStyle = WS_OVERLAPPEDWINDOW;
		DWORD fullScreenStyle = WS_POPUP;

		if (newWindowMode == WindowMode::Fullscreen)
		{
			currentStyle &= ~windowedStyle;
			currentStyle |= fullScreenStyle;

			GetWindowPlacement(window, &previousPlacement);

			SetWindowLong(window, GWL_STYLE, currentStyle);
			//UINT flags = SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOCOPYBITS;

			SetWindowPos(window, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

			HMONITOR monitor = MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY /*MONITOR_DEFAULTTONDEAREST (BorderedFullscreen)*/);
			MONITORINFO monInfo = {};
			monInfo.cbSize = sizeof(MONITORINFO);
			GetMonitorInfo(monitor, &monInfo);

			width = monInfo.rcMonitor.right - monInfo.rcMonitor.left;
			height = monInfo.rcMonitor.bottom - monInfo.rcMonitor.top;
			LONG x = monInfo.rcMonitor.left;
			LONG y = monInfo.rcMonitor.top;

			SetWindowPos(window, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSENDCHANGING);

			//ShowWindow(hWnd, SW_RESTORE);
		}
		else
		{
			currentStyle &= ~fullScreenStyle;
			currentStyle |= windowedStyle;
			SetWindowLong(window, GWL_STYLE, currentStyle);
			SetWindowPos(window, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
			SetWindowPlacement(window, &previousPlacement);
		}
	}
}

void Window::SetAsActiveWindow()
{
}

bool Window::IsActive() const
{
	return isActive;
}

void* Window::GetWindowHandle() const
{
	return window;
}
