// Copyright 2020, Nathan Blane

#include "Platform/Platform.hpp"
#include <xinput.h>

#include "MusaAppWindows.hpp"
#include "Window/Window.h"
#include "Debugging/MetricInterface.hpp"
#include "Input/Internal/InputInternal.hpp"
#include "Input/InputDefinitions.hpp"
#include "Input/Internal/ControllerInputUtilities.hpp"
#include "Math/MathFunctions.hpp"
#include "Containers/MemoryBuffer.hpp"

DECLARE_METRIC_GROUP(WindowsInput);
METRIC_STAT(PumpMessages, WindowsInput);

constexpr u32 WindowsKeyInputCount = 0x20c;
static StaticArray<Inputs::Type, WindowsKeyInputCount> windowsInputs;

static void InitializeWindowsInputArray()
{
	for (auto& input : windowsInputs)
	{
		input = Inputs::_INPUT_ENUM_MAX_;
	}

	// Store 0-9 keys
	constexpr u32 key0 = 0x30;
	for (u32 i = 0; i < 10; ++i)
	{
		windowsInputs[key0 + i] = (Inputs::Type)(Inputs::Key_0 + i);
	}

	// Store A-Z keys
	constexpr u32 keyA = 0x41;
	for (u32 i = 0; i < 26; ++i)
	{
		windowsInputs[keyA + i] = (Inputs::Type)(Inputs::Key_A + i);
	}

	// Store Mouse Button inputs
	windowsInputs[VK_LBUTTON] = Inputs::Mouse_LeftButton;
	windowsInputs[VK_RBUTTON] = Inputs::Mouse_RightButton;
	windowsInputs[VK_MBUTTON] = Inputs::Mouse_MiddleButton;
	windowsInputs[VK_XBUTTON1] = Inputs::Mouse_Button4;
	windowsInputs[VK_XBUTTON2] = Inputs::Mouse_Button5;

	// Store Alt, Shift, Control, Caps, Tab, Esc
	windowsInputs[VK_LSHIFT] = Inputs::Key_LeftShift;
	windowsInputs[VK_RSHIFT] = Inputs::Key_RightShift;
	windowsInputs[VK_LCONTROL] = Inputs::Key_LeftControl;
	windowsInputs[VK_RCONTROL] = Inputs::Key_RightControl;
	windowsInputs[VK_LMENU] = Inputs::Key_LeftAlt;
	windowsInputs[VK_RMENU] = Inputs::Key_RightAlt;
	windowsInputs[VK_CAPITAL] = Inputs::Key_Capslock;
	windowsInputs[VK_SPACE] = Inputs::Key_Space;
	windowsInputs[VK_TAB] = Inputs::Key_Tab;
	windowsInputs[VK_ESCAPE] = Inputs::Key_Escape;

	// Store punctuation
	windowsInputs[186] = Inputs::Key_Semicolon;
	windowsInputs[187] = Inputs::Key_Equal;
	windowsInputs[188] = Inputs::Key_Comma;
	windowsInputs[189] = Inputs::Key_Minus;
	windowsInputs[190] = Inputs::Key_Period;
	windowsInputs[191] = Inputs::Key_ForwardSlash;
	windowsInputs[192] = Inputs::Key_Tilde;
	windowsInputs[219] = Inputs::Key_LeftBracket;
	windowsInputs[220] = Inputs::Key_Backslash;
	windowsInputs[221] = Inputs::Key_RightBracket;
	windowsInputs[222] = Inputs::Key_Apostrophe;

	// Store Function keys
	for (u32 i = 0; i < 12; ++i)
	{
		windowsInputs[VK_F1 + i] = (Inputs::Type)(Inputs::Key_F1 + i);
	}

	// Store Arrow keys
	windowsInputs[VK_LEFT] = Inputs::Key_ArrowLeft;
	windowsInputs[VK_RIGHT] = Inputs::Key_ArrowRight;
	windowsInputs[VK_UP] = Inputs::Key_ArrowUp;
	windowsInputs[VK_DOWN] = Inputs::Key_ArrowDown;

	// Store Input keys
	windowsInputs[VK_RETURN] = Inputs::Key_Enter;
	windowsInputs[VK_BACK] = Inputs::Key_Backspace;
	windowsInputs[VK_PRIOR] = Inputs::Key_PageUp;
	windowsInputs[VK_NEXT] = Inputs::Key_PageDown;
	windowsInputs[VK_INSERT] = Inputs::Key_Insert;
	windowsInputs[VK_DELETE] = Inputs::Key_Delete;
	windowsInputs[VK_HOME] = Inputs::Key_Home;
	windowsInputs[VK_END] = Inputs::Key_End;

	// Store numpad keys
	windowsInputs[VK_NUMLOCK] = Inputs::Key_NumLock;
	for (u32 i = 0; i < 10; ++i)
	{
		windowsInputs[VK_NUMPAD0 + i] = (Inputs::Type)(Inputs::Key_Num0 + i);
	}
	windowsInputs[VK_MULTIPLY] = Inputs::Key_NumMulti;
	windowsInputs[VK_ADD] = Inputs::Key_NumPlus;
	windowsInputs[VK_SUBTRACT] = Inputs::Key_NumMinus;
	windowsInputs[VK_DECIMAL] = Inputs::Key_NumDecimal;
	windowsInputs[VK_DIVIDE] = Inputs::Key_NumDivide;

	// Store Unique keys
	windowsInputs[VK_SNAPSHOT] = Inputs::Key_PrintScreen;
	windowsInputs[VK_SCROLL] = Inputs::Key_ScrollLock;
	windowsInputs[VK_PAUSE] = Inputs::Key_Pause;

}

WPARAM MapWparamLeftRightKeys(WPARAM wparam, LPARAM lparam)
{
	u32 scanCode = (lparam & 0x00ff0000) >> 16;
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

Inputs::Type ConvertWin32ToMusaInput(u32 vkCode)
{
	// TODO - this could be cached to be a static array of initialized vk codes
	return windowsInputs[vkCode];
}

inline Inputs::Type GetMouseType(UINT message, WPARAM wParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN: return Inputs::Mouse_LeftButton;
	case WM_RBUTTONDOWN: return Inputs::Mouse_RightButton;
	case WM_MBUTTONDOWN: return Inputs::Mouse_MiddleButton;
	case WM_LBUTTONUP: return Inputs::Mouse_LeftButton;
	case WM_RBUTTONUP: return Inputs::Mouse_RightButton;
	case WM_MBUTTONUP: return Inputs::Mouse_MiddleButton;

	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	{
		return (HIWORD(wParam) & XBUTTON1) ? Inputs::Mouse_Button4 : Inputs::Mouse_Button5;
	}

	default:
		return Inputs::_INPUT_ENUM_MAX_;
	}
}

// Basic Window callback
LRESULT CALLBACK WindowCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WindowInputHandler* inputHandler = (WindowInputHandler*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if(inputHandler != nullptr)
	{
		switch (message)
		{
			case WM_ACTIVATEAPP:
			{
				// Deactivating the app allows for the mouse to move freely again. This is done because the app isn't "active"
				// Because of this, most of the input behavior won't happen, like setting the position to be the middle of the screen 
				// or whatever every frame. This is the main thing that's prevented the mouse from moving outside of the engine....

				bool activated = wParam;
				inputHandler->HandleActivationChanged(activated);
			}break;

			case  WM_CLOSE:
			{
				inputHandler->HandleWindowClose();
			}break;

			case WM_DESTROY:
			{
			}break;

			case WM_SIZE:
			{
				u32 width = LOWORD(lParam);
				u32 height = HIWORD(lParam);
				inputHandler->HandleWindowResized(width, height);
			}break;

			case WM_SETCURSOR:
			{
				// If the DefWindowProc processes this, it doesn't update the cursor for some reason
				return 0;
			}

			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_XBUTTONDOWN:
			{
				Inputs::Type mouseButton = GetMouseType(message, wParam);
				inputHandler->HandleMouseDown(mouseButton);
			}break;

			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			case WM_MBUTTONUP:
			case WM_XBUTTONUP:
			{
				Inputs::Type mouseButton = GetMouseType(message, wParam);
				inputHandler->HandleMouseUp(mouseButton);
			}break;

			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
			{
				wParam = MapWparamLeftRightKeys(wParam, lParam);
				u32 vkCode = LOWORD(wParam);

				bool repeated = (lParam & 0x40000000) != 0;

				Inputs::Type input = ConvertWin32ToMusaInput(vkCode);
				inputHandler->HandleKeyDown(input, repeated);

			}break;

			case WM_SYSKEYUP:
			case WM_KEYUP:
			{
				wParam = MapWparamLeftRightKeys(wParam, lParam);
				u32 vkCode = LOWORD(wParam);
				
				Inputs::Type input = ConvertWin32ToMusaInput(vkCode);
				Assert(input != Inputs::_INPUT_ENUM_MAX_);

				inputHandler->HandleKeyUp(input);
			}break;

			case WM_CHAR:
			{
				// TODO - Support Unicode characters!
				char c = (tchar)wParam;

				// 30th bit containing the prev state of the character...
				bool repeated = (lParam & 0x40000000) != 0;
				
				inputHandler->HandleKeyChar(c, repeated);
			}break;

			// According to multiple sources, this kind of mouse movement message when the mouse is hiddedn because:
			//   1) It has more resolution for mouse movement
			//   2) It essentially only has delta information
			// Because of these reasons, I will be using this every time the mouse is hidden on Windows
			case WM_INPUT:
			{
				UINT size;
				::GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));

				MemoryBuffer riData(size);
				if (::GetRawInputData((HRAWINPUT)lParam, RID_INPUT, riData.GetData(), &size, sizeof(RAWINPUTHEADER)) == size)
				{
					const RAWINPUT* rawInput = reinterpret_cast<RAWINPUT*>(riData.GetData());

					if (rawInput->header.dwType == RIM_TYPEMOUSE)
					{
						POINT cursorPos;
						::GetCursorPos(&cursorPos);
						const i32 deltaX = rawInput->data.mouse.lLastX;
						const i32 deltaY = rawInput->data.mouse.lLastY;
						inputHandler->HandleRawMouseMove(cursorPos.x, cursorPos.y, deltaX, deltaY);
					}
				}
			}break;

			case WM_MOUSEMOVE:
			{
				POINT cursor;
				::GetCursorPos(&cursor);
				inputHandler->HandleMouseMove(cursor.x, cursor.y);
			}break;

			default:
			{
			} break;
		}
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////

static const Inputs::Type ControllerButtons[] = {
	Inputs::Gamepad_DPadUp,
	Inputs::Gamepad_DPadDown,
	Inputs::Gamepad_DPadLeft,
	Inputs::Gamepad_DPadRight,
	Inputs::Gamepad_StartButton,
	Inputs::Gamepad_SelectButton,
	Inputs::Gamepad_LeftShoulder,
	Inputs::Gamepad_RightShoulder,
	Inputs::Gamepad_AButton,
	Inputs::Gamepad_BButton,
	Inputs::Gamepad_XButton,
	Inputs::Gamepad_YButton,
};

void ProcessAnalogControllerInputs(WindowInputHandler& inputHandler, u32 controllerIndex, const XINPUT_GAMEPAD& xinputGamepad, WindowsGamepadState& state)
{
	// Deadzone checking
	if (//state.leftStick.x != xinputGamepad.sThumbLX ||
		Math::Abs(xinputGamepad.sThumbLX) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		f32 normLX = NormalizeStickValue(xinputGamepad.sThumbLX);
		inputHandler.HandleControllerAnalogChange(controllerIndex, Inputs::Gamepad_LeftStick_XAxis, normLX);
		state.leftStick.x = xinputGamepad.sThumbLX;
	}

	if (//state.leftStick.y != xinputGamepad.sThumbLY ||
		Math::Abs(xinputGamepad.sThumbLY) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		f32 normLY = NormalizeStickValue(xinputGamepad.sThumbLY);
		inputHandler.HandleControllerAnalogChange(controllerIndex, Inputs::Gamepad_LeftStick_YAxis, normLY);
		state.leftStick.y = xinputGamepad.sThumbLY;
	}

	if (//state.rightStick.x != xinputGamepad.sThumbRX ||
		Math::Abs(xinputGamepad.sThumbRX) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		f32 normRX = NormalizeStickValue(xinputGamepad.sThumbRX);
		inputHandler.HandleControllerAnalogChange(controllerIndex, Inputs::Gamepad_RightStick_XAxis, normRX);
		state.rightStick.x = xinputGamepad.sThumbRX;
	}

	if (//state.rightStick.y != xinputGamepad.sThumbRY ||
		Math::Abs(xinputGamepad.sThumbRY) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		f32 normRY = NormalizeStickValue(xinputGamepad.sThumbRY);
		inputHandler.HandleControllerAnalogChange(controllerIndex, Inputs::Gamepad_RightStick_YAxis, normRY);
		state.rightStick.y = xinputGamepad.sThumbRY;
	}

	// Triggers for axis analog
	if (Math::Abs(xinputGamepad.bLeftTrigger) > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		f32 normLeftTrigger = NormalizeTriggerValue(xinputGamepad.bLeftTrigger);
		inputHandler.HandleControllerAnalogChange(controllerIndex, Inputs::Gamepad_LeftTrigger, normLeftTrigger);
	}
	if (Math::Abs(xinputGamepad.bRightTrigger) > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		f32 normRightTrigger = NormalizeTriggerValue(xinputGamepad.bRightTrigger);
		inputHandler.HandleControllerAnalogChange(controllerIndex, Inputs::Gamepad_RightTrigger, normRightTrigger);
	}
}

void ProcessControllerButtons(WindowInputHandler& inputHandler, u32 controllerIndex, const XINPUT_GAMEPAD& xinputGamepad, WindowsGamepadState& state)
{
	StaticArray<bool, MaxSupportedControllerButtons> currentButtonState;
	currentButtonState[0] = (xinputGamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0;
	currentButtonState[1] = (xinputGamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
	currentButtonState[2] = (xinputGamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0;
	currentButtonState[3] = (xinputGamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0;
	currentButtonState[4] = (xinputGamepad.wButtons & XINPUT_GAMEPAD_START) != 0;
	currentButtonState[5] = (xinputGamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0;
	currentButtonState[6] = (xinputGamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0;
	currentButtonState[7] = (xinputGamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
	currentButtonState[8] = (xinputGamepad.wButtons & XINPUT_GAMEPAD_A) != 0;
	currentButtonState[9] = (xinputGamepad.wButtons & XINPUT_GAMEPAD_B) != 0;
	currentButtonState[10] = (xinputGamepad.wButtons & XINPUT_GAMEPAD_X) != 0;
	currentButtonState[11] = (xinputGamepad.wButtons & XINPUT_GAMEPAD_Y) != 0;

	for (u32 i = 0; i < MaxSupportedControllerButtons; ++i)
	{
		if (currentButtonState[i] != state.buttonStates[i])
		{
			if (currentButtonState[i])
			{
				inputHandler.HandleControllerButtonDown(controllerIndex, ControllerButtons[i]);
			}
			else
			{
				inputHandler.HandleControllerButtonUp(controllerIndex, ControllerButtons[i]);
			}
		}
		else
		{
			if (currentButtonState[i])
			{
				inputHandler.HandleControllerButtonDown(controllerIndex, ControllerButtons[i]);
			}
		}

		state.buttonStates[i] = currentButtonState[i];
	}
}

//////////////////////////////////////////////////////////////////////////
// MusaAppWindows
//////////////////////////////////////////////////////////////////////////

MusaAppWindows::MusaAppWindows(UniquePtr<WindowInputHandler>&& inputHandler)
	: MusaAppOS(std::move(inputHandler))
{
	instance = (HINSTANCE)GetModuleHandle(nullptr);

	WNDCLASSEX windowClass = {};
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpfnWndProc = WindowCallback;
	windowClass.hInstance = instance;
	// TODO - Store this somewhere that can be accessed by the window that's created
	windowClass.lpszClassName = TEXT("MusaEngine");
	windowClass.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&windowClass))
	{
		Assert(false);
	}

	InitializeWindowsInputArray();
}

Window* MusaAppWindows::CreateGameWindow(u32 xPos, u32 yPos, u32 width, u32 height)
{
	return new Window(instance, *inputHandler, xPos, yPos, width, height);
}

void MusaAppWindows::SetRawMouseInput(bool enabled, const Window& window)
{
	DWORD riFlags = enabled ? 0 : RIDEV_REMOVE;
	HWND wnd = enabled ? reinterpret_cast<HWND>(window.GetWindowHandle()) : nullptr;

	constexpr u32 riMouseUsage = 2;
	RAWINPUTDEVICE riDev = {};
	riDev.dwFlags = riFlags;
	riDev.hwndTarget = wnd;
	riDev.usUsage = riMouseUsage;
	riDev.usUsagePage = 1;

	NOT_USED BOOL registered = ::RegisterRawInputDevices(&riDev, 1, sizeof(RAWINPUTDEVICE));
	Assert(registered);
}

void MusaAppWindows::ShowCursor(bool showCursor)
{
	::ShowCursor(showCursor);
}

void MusaAppWindows::SetMousePosition(const IntVector2& mousePos)
{
	::SetCursorPos(mousePos.x, mousePos.y);
}

IntVector2 MusaAppWindows::GetMousePosition() const
{
	POINT cursorPos;
	::GetCursorPos(&cursorPos);

	return IntVector2(cursorPos.x, cursorPos.y);
}

void MusaAppWindows::LockCursorToRect(const IntRect& rect)
{
	RECT r = {};
	r.left = rect.x;
	r.top = rect.y;
	r.right = rect.x + rect.width;
	r.bottom = rect.y + rect.height;
	::ClipCursor(&r);
}

void MusaAppWindows::UnlockCursorFromRect()
{
	::ClipCursor(nullptr);
}

void MusaAppWindows::ProcessNativeGamepad()
{
	// NOTE - I could check if controllers are actually connected? Don't really know what that gets me though

	// NOTE - This amount of controllers might not be supported on other platforms. Might need to know this info
	constexpr u32 MaxControllersSupported = 1;//XUSER_MAX_COUNT;
	for (u32 i = 0; i < MaxControllersSupported; ++i)
	{
		XINPUT_STATE state = {};
		if (XInputGetState(i, &state) == ERROR_SUCCESS)
		{
			controllers.activeControllers[i] = true;

			WindowsGamepadState& musaGamepad = controllers.controllerStates[i];
			const XINPUT_GAMEPAD& gamepad = state.Gamepad;

			// Stick processing
			ProcessAnalogControllerInputs(*inputHandler, i, gamepad, musaGamepad);

			// Button processing
			ProcessControllerButtons(*inputHandler, i, gamepad, musaGamepad);
		}
		else
		{
			// Controller not connected...
			controllers.activeControllers[i] = false;
		}
	}
}

void MusaAppWindows::ProcessInputEvents()
{
	SCOPED_TIMED_BLOCK(PumpMessages);
	MSG Message;
	while (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
}

