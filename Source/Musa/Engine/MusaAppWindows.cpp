
#include "Platform.h"
#include <xinput.h>

#include "MusaAppWindows.hpp"
#include "Window/Window.h"
#include "Debugging/MetricInterface.hpp"
#include "Input/Internal/InputInternal.hpp"
#include "Input/InputDefinitions.hpp"
#include "Input/Internal/ControllerInputUtilities.hpp"
#include "Math/MathUtilities.hpp"

DECLARE_METRIC_GROUP(WindowsInput);
METRIC_STAT(PumpMessages, WindowsInput);


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
	// TODO - this could be cached to be a static array of initialized vk codes
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

Inputs::Type GetMouseType(WPARAM wParam)
{
	switch (wParam)
	{
		case MK_LBUTTON: return Inputs::Mouse_LeftButton;
		case MK_RBUTTON: return Inputs::Mouse_RightButton;
		case MK_MBUTTON: return Inputs::Mouse_MiddleButton;
		case MK_XBUTTON1: return Inputs::Mouse_Button4;
		case MK_XBUTTON2: return Inputs::Mouse_Button5;
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
				uint32 width = LOWORD(lParam);
				uint32 height = HIWORD(lParam);
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
			{
				Inputs::Type mouseButton = GetMouseType(wParam);
				inputHandler->HandleMouseDown(mouseButton);
			}break;

			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			case WM_MBUTTONUP:
			{
				Inputs::Type mouseButton = GetMouseType(wParam);
				inputHandler->HandleMouseUp(mouseButton);
			}break;

			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
			{
				wParam = MapWparamLeftRightKeys(wParam, lParam);
				uint32 vkCode = LOWORD(wParam);

				bool repeated = (lParam & 0x40000000) != 0;

				Inputs::Type input = ConvertWin32ToMusaInput(vkCode);
				inputHandler->HandleKeyDown(input, repeated);

			}break;

			case WM_SYSKEYUP:
			case WM_KEYUP:
			{
				uint32 vkCode = LOWORD(wParam);
				wParam = MapWparamLeftRightKeys(wParam, lParam);
				Inputs::Type input = ConvertWin32ToMusaInput(vkCode);

				inputHandler->HandleKeyUp(input);
			}break;

			case WM_CHAR:
			{
				//inputHandler->HandleKeyChar();
			}break;

			// According to multiple sources, this kind of mouse movement message when the mouse is hiddedn because:
			//   1) It has more resolution for mouse movement
			//   2) It essentially only has delta information
			// Because of these reasons, I will be using this every time the mouse is hidden on Windows
			case WM_INPUT:
			{
				UINT size;
				::GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));

				DynamicArray<uint8> riData(size);
				if (::GetRawInputData((HRAWINPUT)lParam, RID_INPUT, riData.GetData(), &size, sizeof(RAWINPUTHEADER)) == size)
				{
					const RAWINPUT* rawInput = reinterpret_cast<RAWINPUT*>(riData.GetData());

					if (rawInput->header.dwType == RIM_TYPEMOUSE)
					{
						POINT cursorPos;
						::GetCursorPos(&cursorPos);
						const int32 deltaX = rawInput->data.mouse.lLastX;
						const int32 deltaY = rawInput->data.mouse.lLastY;
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

	return DefWindowProc(hwnd, message, wParam, lParam);;
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

void ProcessAnalogControllerInputs(WindowInputHandler& inputHandler, uint32 controllerIndex, const XINPUT_GAMEPAD& xinputGamepad, WindowsGamepadState& state)
{
	// Deadzone checking
	if (//state.leftStick.x != xinputGamepad.sThumbLX ||
		Math::Abs(xinputGamepad.sThumbLX) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		float32 normLX = NormalizeStickValue(xinputGamepad.sThumbLX);
		inputHandler.HandleControllerAnalogChange(controllerIndex, Inputs::Gamepad_LeftStick_XAxis, normLX);
		state.leftStick.x = xinputGamepad.sThumbLX;
	}

	if (//state.leftStick.y != xinputGamepad.sThumbLY ||
		Math::Abs(xinputGamepad.sThumbLY) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		float32 normLY = NormalizeStickValue(xinputGamepad.sThumbLY);
		inputHandler.HandleControllerAnalogChange(controllerIndex, Inputs::Gamepad_LeftStick_YAxis, normLY);
		state.leftStick.y = xinputGamepad.sThumbLY;
	}

	if (//state.rightStick.x != xinputGamepad.sThumbRX ||
		Math::Abs(xinputGamepad.sThumbRX) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		float32 normRX = NormalizeStickValue(xinputGamepad.sThumbRX);
		inputHandler.HandleControllerAnalogChange(controllerIndex, Inputs::Gamepad_RightStick_XAxis, normRX);
		state.rightStick.x = xinputGamepad.sThumbRX;
	}

	if (//state.rightStick.y != xinputGamepad.sThumbRY ||
		Math::Abs(xinputGamepad.sThumbRY) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		float32 normRY = NormalizeStickValue(xinputGamepad.sThumbRY);
		inputHandler.HandleControllerAnalogChange(controllerIndex, Inputs::Gamepad_RightStick_YAxis, normRY);
		state.rightStick.y = xinputGamepad.sThumbRY;
	}

	// Triggers for axis analog
	if (Math::Abs(xinputGamepad.bLeftTrigger) > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		float32 normLeftTrigger = NormalizeTriggerValue(xinputGamepad.bLeftTrigger);
		inputHandler.HandleControllerAnalogChange(controllerIndex, Inputs::Gamepad_LeftTrigger, normLeftTrigger);
	}
	if (Math::Abs(xinputGamepad.bRightTrigger) > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		float32 normRightTrigger = NormalizeTriggerValue(xinputGamepad.bRightTrigger);
		inputHandler.HandleControllerAnalogChange(controllerIndex, Inputs::Gamepad_RightTrigger, normRightTrigger);
	}
}

void ProcessControllerButtons(WindowInputHandler& inputHandler, uint32 controllerIndex, const XINPUT_GAMEPAD& xinputGamepad, WindowsGamepadState& state)
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

	for (uint32 i = 0; i < MaxSupportedControllerButtons; ++i)
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
}

Window* MusaAppWindows::CreateGameWindow(uint32 xPos, uint32 yPos, uint32 width, uint32 height)
{
	return new Window(instance, *inputHandler, xPos, yPos, width, height);
}

void MusaAppWindows::SetRawMouseInput(bool enabled, const Window& window)
{
	DWORD riFlags = enabled ? 0 : RIDEV_REMOVE;
	HWND wnd = enabled ? reinterpret_cast<HWND>(window.GetWindowHandle()) : nullptr;

	constexpr uint32 riMouseUsage = 2;
	RAWINPUTDEVICE riDev = {};
	riDev.dwFlags = riFlags;
	riDev.hwndTarget = wnd;
	riDev.usUsage = riMouseUsage;
	riDev.usUsagePage = 1;

	BOOL registered = ::RegisterRawInputDevices(&riDev, 1, sizeof(RAWINPUTDEVICE));
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
	constexpr uint32 MaxControllersSupported = 1;//XUSER_MAX_COUNT;
	for (uint32 i = 0; i < MaxControllersSupported; ++i)
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

