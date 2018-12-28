#include "EngineCore/Platform.h"
#include "Windowing/Window.h"
#include "Input/InputDefinitions.h"
#include "Input/InputManager.h"

// Basic Window callback
LRESULT CALLBACK WindowCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	switch (message)
	{
		case  WM_CLOSE:
		{
			window->Close();
		}break;

		case WM_DESTROY:
		{
			window->Close();
		}break;

		case WM_SIZE:
		{
			window->Resize();
			// 			if (hasInitialized)
			// 			{
			// 				RECT windowRect = {};
			// 				GetClientRect(window, &windowRect);
			// 				Width = windowRect.right - windowRect.left;
			// 				Height = windowRect.bottom - windowRect.top;
			// 
			// 				recreateSwapchain(vulkan);
			// 			}
		}break;

		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			uint32 vkCode = static_cast<uint32>(wParam);

			bool repeated = (lParam & 0x40000000) != 0;
			//printf("%d down -- repeated?: %s\n", vkCode, repeated ? "true" : "false");

			//bool altWasDown = !!(lParam & (1 << 29));
			//altMod = altWasDown;

			bool isPressed = (lParam & (1 << 31)) == 0;
			Internal::KeyMessageDownReceived(vkCode, isPressed, repeated);

			if (vkCode == KeyInput::Key_Escape)
			{
				window->Close();
			}

		}break;

		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			uint32 vkCode = static_cast<uint32>(wParam);

			//printf("%d up\n", vkCode);

			//bool altWasDown = !!(lParam & (1 << 29));
			//altMod = altWasDown;

			Internal::KeyMessageUpReceived(vkCode);
		}break;

		case WM_MOUSEMOVE:
		{
			Internal::MouseMovementChange();
			
			RECT windowRect;
			GetWindowRect((HWND)window->GetWindowHandle(), &windowRect);
			int32 originX = windowRect.left;
			int32 originY = windowRect.top;

			int32 centerX = originX + (window->GetWidth() / 2);
			int32 centerY = originY + (window->GetHeight() / 2);
			SetCursorPos(centerX, centerY);
		}break;

		default:
		{
			result = DefWindowProc(hwnd, message, wParam, lParam);
		} break;
	}

	return result;
}


Window::Window(int32 w, int32 h)
	: window(nullptr),
	width(w),
	height(h),
	isShown(false)
{
}

Window::~Window()
{
	assert(window != nullptr);
	DestroyWindow(window);
}

void Window::Initialize()
{
	HINSTANCE Instance = (HINSTANCE)GetModuleHandle(nullptr);

	WNDCLASSEX windowClass = {};
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpfnWndProc = WindowCallback;
	windowClass.hInstance = Instance;
	windowClass.lpszClassName = TEXT("EngineWindow");
	windowClass.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&windowClass))
	{
		assert(false);
	}

	DWORD style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
	DWORD exStyle = 0;
	width = width;
	height = height;
	window = CreateWindowEx(exStyle,
		windowClass.lpszClassName, TEXT("Musa Engine Window"),
		style,
		CW_USEDEFAULT, CW_USEDEFAULT,
		width, height,
		nullptr, nullptr, Instance, nullptr);

	if (!window)
	{
		assert(false);
	}

	SetWindowLongPtr(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	RECT windowRect;
	GetWindowRect(window, &windowRect);
	int32 originX = windowRect.left;
	int32 originY = windowRect.top;

	Show();
	ShowCursor(FALSE);

	RECT cursorRect = {};
	cursorRect.left = originX;
	cursorRect.top = originY;
	cursorRect.right = width;
	cursorRect.bottom = height;
	ClipCursor(&cursorRect);

	int32 centerX = originX + (width / 2);
	int32 centerY = originY + (height / 2);
	SetCursorPos(centerX, centerY);

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

void Window::Close()
{
	if (isActive)
	{
		isActive = false;
	}
}

void Window::Resize()
{

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

bool Window::PresentationSupported(GPUHandle gpu, uint32 queueIndex) const
{
	return vkGetPhysicalDeviceWin32PresentationSupportKHR((VkPhysicalDevice)gpu, queueIndex);
}

bool Window::IsActive() const
{
	return isActive;
}

void Window::SwapBuffers()
{
	
}

int32 Window::GetWidth() const
{
	return width;
}

int32 Window::GetHeight() const
{
	return height;
}

void* Window::GetWindowHandle() const
{
	return window;
}
