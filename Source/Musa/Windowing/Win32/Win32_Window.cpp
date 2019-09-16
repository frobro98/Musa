#include "EngineCore/Platform.h"
#include "Windowing/Window.h"
#include "InputDefinitions.hpp"
#include "Input/Internal/InputInternal.hpp"

WPARAM MapWparamLeftRightKeys(WPARAM wparam, LPARAM lparam)
{
	uint32 scanCode = (lparam & 0x00ff0000) >> 16;
	bool extended = (lparam & 0x01000000) != 0;
	switch (wparam)
	{
	case VK_SHIFT:
		return MapVirtualKey(scanCode, MAPVK_VSC_TO_VK_EX);
	case VK_CONTROL:
		return extended ? VK_RCONTROL : VK_LCONTROL;
	case VK_MENU:
		return extended ? VK_RMENU : VK_LMENU;
	default:
		return wparam;
	}
}

Inputs::Type ConvertWin32ToMusaInput(uint32 vkCode)
{
	switch (vkCode)
	{
	case 0x41: return Inputs::Key_A;
	case 0x42: return Inputs::Key_B;
	case 0x43: return Inputs::Key_C;
	case 0x44: return Inputs::Key_D;
	case 0x45: return Inputs::Key_E;
	case 0x46: return Inputs::Key_F;
	case 0x47: return Inputs::Key_G;
	case 0x48: return Inputs::Key_H;
	case 0x49: return Inputs::Key_I;
	case 0x4a: return Inputs::Key_J;
	case 0x4b: return Inputs::Key_K;
	case 0x4c: return Inputs::Key_L;
	case 0x4d: return Inputs::Key_M;
	case 0x4e: return Inputs::Key_N;
	case 0x4f: return Inputs::Key_O;
	case 0x50: return Inputs::Key_P;
	case 0x51: return Inputs::Key_Q;
	case 0x52: return Inputs::Key_R;
	case 0x53: return Inputs::Key_S;
	case 0x54: return Inputs::Key_T;
	case 0x55: return Inputs::Key_U;
	case 0x56: return Inputs::Key_V;
	case 0x57: return Inputs::Key_W;
	case 0x58: return Inputs::Key_X;
	case 0x59: return Inputs::Key_Y;
	case 0x5a: return Inputs::Key_Z;
	case 0x30: return Inputs::Key_0;
	case 0x31: return Inputs::Key_1;
	case 0x32: return Inputs::Key_2;
	case 0x33: return Inputs::Key_3;
	case 0x34: return Inputs::Key_4;
	case 0x35: return Inputs::Key_5;
	case 0x36: return Inputs::Key_6;
	case 0x37: return Inputs::Key_7;
	case 0x38: return Inputs::Key_8;
	case 0x39: return Inputs::Key_9;
	case VK_LBUTTON: return Inputs::Mouse_LeftButton;
	case VK_RBUTTON: return Inputs::Mouse_RightButton;
	case VK_MBUTTON: return Inputs::Mouse_MiddleButton;
	case VK_XBUTTON1: return Inputs::Mouse_Button4;
	case VK_XBUTTON2: return Inputs::Mouse_Button5;
	case VK_LSHIFT: return Inputs::Key_LeftShift;
	case VK_RSHIFT: return Inputs::Key_RightShift;
	case VK_LCONTROL: return Inputs::Key_LeftControl;
	case VK_RCONTROL: return Inputs::Key_RightControl;
	case VK_LMENU: return Inputs::Key_LeftAlt;
	case VK_RMENU: return Inputs::Key_RightAlt;
	case VK_CAPITAL: return Inputs::Key_Capslock;
	case VK_SPACE: return Inputs::Key_Space;
	case VK_TAB: return Inputs::Key_Tab;
	case VK_ESCAPE: return Inputs::Key_Escape;
	case 186: return Inputs::Key_Semicolon;
	case 187: return Inputs::Key_Equal;
	case 188: return Inputs::Key_Comma;
	case 189: return Inputs::Key_Minus;
	case 190: return Inputs::Key_Period;
	case 191: return Inputs::Key_ForwardSlash;
	case 192: return Inputs::Key_Tilde;
	case 219: return Inputs::Key_LeftBracket;
	case 220: return Inputs::Key_Backslash;
	case 221: return Inputs::Key_RightBracket;
	case 222: return Inputs::Key_Apostrophe;

	// Function keys
	case VK_F1: return Inputs::Key_F1;
	case VK_F2: return Inputs::Key_F2;
	case VK_F3: return Inputs::Key_F3;
	case VK_F4: return Inputs::Key_F4;
	case VK_F5: return Inputs::Key_F5;
	case VK_F6: return Inputs::Key_F6;
	case VK_F7: return Inputs::Key_F7;
	case VK_F8: return Inputs::Key_F8;
	case VK_F9: return Inputs::Key_F9;
	case VK_F10: return Inputs::Key_F10;
	case VK_F11: return Inputs::Key_F11;
	case VK_F12: return Inputs::Key_F12;

	// Arrow keys
	case VK_LEFT: return Inputs::Key_ArrowLeft;
	case VK_RIGHT: return Inputs::Key_ArrowRight;
	case VK_UP: return Inputs::Key_ArrowUp;
	case VK_DOWN: return Inputs::Key_ArrowDown;

	// Input keys
	case VK_RETURN: return Inputs::Key_Enter;
	case VK_BACK: return Inputs::Key_Backspace;
	case VK_PRIOR: return Inputs::Key_PageUp;
	case VK_NEXT: return Inputs::Key_PageDown;
	case VK_INSERT: return Inputs::Key_Insert;
	case VK_DELETE: return Inputs::Key_Delete;
	case VK_HOME: return Inputs::Key_Home;
	case VK_END: return Inputs::Key_End;

	case VK_NUMLOCK: return Inputs::Key_NumLock;
	case VK_NUMPAD0: return Inputs::Key_Num0;
	case VK_NUMPAD1: return Inputs::Key_Num1;
	case VK_NUMPAD2: return Inputs::Key_Num2;
	case VK_NUMPAD3: return Inputs::Key_Num3;
	case VK_NUMPAD4: return Inputs::Key_Num4;
	case VK_NUMPAD5: return Inputs::Key_Num5;
	case VK_NUMPAD6: return Inputs::Key_Num6;
	case VK_NUMPAD7: return Inputs::Key_Num7;
	case VK_NUMPAD8: return Inputs::Key_Num8;
	case VK_NUMPAD9: return Inputs::Key_Num9;
	case VK_MULTIPLY: return Inputs::Key_NumMulti;
	case VK_ADD: return Inputs::Key_NumPlus;
	case VK_SUBTRACT: return Inputs::Key_NumMinus;
	case VK_DECIMAL: return Inputs::Key_NumDecimal;
	case VK_DIVIDE: return Inputs::Key_NumDivide;

	// Unique keys
	case VK_SNAPSHOT: return Inputs::Key_PrintScreen;
	case VK_SCROLL: return Inputs::Key_ScrollLock;
	case VK_PAUSE: return Inputs::Key_Pause;
	default:
		return Inputs::_INPUT_ENUM_MAX_;
	}
}

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

			bool isPressed = (lParam & (1 << 31)) == 0;
			wParam = MapWparamLeftRightKeys(wParam, lParam);
			Inputs::Type input = ConvertWin32ToMusaInput(vkCode);
			Internal::KeyMessageDownReceived(input, isPressed, repeated);

			if (vkCode == (uint32)KeyInput::Key_Escape)
			{
				window->Close();
			}
		}break;

		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			uint32 vkCode = static_cast<uint32>(wParam);
			wParam = MapWparamLeftRightKeys(wParam, lParam);
			Inputs::Type input = ConvertWin32ToMusaInput(vkCode);
			Internal::KeyMessageUpReceived(input);
		}break;

		case WM_MOUSEMOVE:
		{
			POINT cursorPos;
			::GetCursorPos(&cursorPos);
			Internal::MouseMovementChange(cursorPos.x, cursorPos.y);
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
	Assert(window != nullptr);
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
		Assert(false);
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
		Assert(false);
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
