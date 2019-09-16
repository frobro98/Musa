#pragma once

#include "PlatformDefinitions.h"

struct Inputs
{
	enum Type
	{
		Key_A,
		Key_B,
		Key_C,
		Key_D,
		Key_E,
		Key_F,
		Key_G,
		Key_H,
		Key_I,
		Key_J,
		Key_K,
		Key_L,
		Key_M,
		Key_N,
		Key_O,
		Key_P,
		Key_Q,
		Key_R,
		Key_S,
		Key_T,
		Key_U,
		Key_V,
		Key_W,
		Key_X,
		Key_Y,
		Key_Z,

		Key_0,
		Key_1,
		Key_2,
		Key_3,
		Key_4,
		Key_5,
		Key_6,
		Key_7,
		Key_8,
		Key_9,

		Key_Tilde,
		Key_Semicolon,
		Key_Comma,
		Key_Period,
		Key_ForwardSlash,
		Key_Apostrophe,
		Key_LeftBracket,
		Key_RightBracket,
		Key_Backslash,
		Key_Equal,
		Key_Minus,


		Key_Enter,
		Key_Backspace,
		Key_Escape,
		Key_Tab,
		Key_Capslock,
		Key_LeftShift,
		Key_LeftControl,
		Key_LeftAlt,
		Key_Space,
		Key_RightAlt,
		Key_RightControl,
		Key_RightShift,

		Key_PrintScreen,
		Key_ScrollLock,
		Key_Pause,
		Key_Insert,
		Key_Delete,
		Key_Home,
		Key_End,
		Key_PageUp,
		Key_PageDown,
		Key_ArrowUp,
		Key_ArrowDown,
		Key_ArrowLeft,
		Key_ArrowRight,

		Key_NumLock,
		Key_Num0,
		Key_Num1,
		Key_Num2,
		Key_Num3,
		Key_Num4,
		Key_Num5,
		Key_Num6,
		Key_Num7,
		Key_Num8,
		Key_Num9,
		Key_NumEnter,
		Key_NumPlus,
		Key_NumMinus,
		Key_NumMulti,
		Key_NumDivide,
		Key_NumDecimal,

		Key_F1,
		Key_F2,
		Key_F3,
		Key_F4,
		Key_F5,
		Key_F6,
		Key_F7,
		Key_F8,
		Key_F9,
		Key_F10,
		Key_F11,
		Key_F12,

		Mouse_LeftButton,
		Mouse_RightButton,
		Mouse_MiddleButton,
		Mouse_Button4,
		Mouse_Button5,
		Mouse_Button6,
		Mouse_Button7,
		Mouse_Button8,

		Mouse_XAxis,
		Mouse_YAxis,

		Max,

		_INPUT_ENUM_MAX_ = 0x7FFFFFFF
	};
	static_assert(Max == Mouse_YAxis + 1);

	enum State
	{
		Shift = 0x1,
		Control = 0x2,
		Alt = 0x4,

		_INPUT_STATE_MAX_ = 0xFF
	};
};


// TODO - Remove these operators
// inline KeyboardState operator|=(KeyboardState& flag1, KeyboardState flag2)
// {
// 	flag1 = (KeyboardState)((uint32)flag1 | (uint32)flag2);
// 	return flag1;
// }
// 
// inline KeyStateFlags operator&(KeyStateFlags flag1, KeyStateFlags flag2)
// {
// 	return (KeyStateFlags)((uint32)flag1 & (uint32)flag2);
// }
// 
// // inline uint32 operator|(KeyStateFlags flag1, KeyStateFlags flag2)
// // {
// // 	return (uint32)flag1 | (uint32)flag2;
// // }
// 
// inline KeyStateFlags operator|(KeyStateFlags flag1, KeyStateFlags flag2)
// {
// 	return (KeyStateFlags)((uint32)flag1 | (uint32)flag2);
// }
// 
// inline bool operator==(uint32 state, KeyInput flag)
// {
// 	return state == static_cast<uint32>(flag);
// }
