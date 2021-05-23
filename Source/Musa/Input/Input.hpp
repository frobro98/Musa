// Copyright 2020, Nathan Blane

#pragma once

#include "String/String.h"
#include "String/StringView.hpp"
#include "Containers/StaticArray.hpp"
#include "Containers/Map.h"
#include "InputDefinitions.hpp"
#include "Math/Vector2.hpp"
#include "Math/IntVector2.hpp"

class ApplicationInputMap;

namespace Input
{
enum class ButtonEventType
{
	Pressed,
	Repeated,
	Released
};

struct ButtonState
{
	u8 endedDown : 1;
	u8 previouslyDown : 1;
};

constexpr u32 MaxSupportedControllers = 4;

struct GamepadState
{
	// Analog fields, unnormalized
	Vector2 leftStick;
	Vector2 rightStick;
	f32 leftTrigger;
	f32 rightTrigger;

	// Button frame state
	StaticArray<ButtonState, Input::GP_Max> buttonStates;
	bool active;
};

using StateMap = StaticArray<Input::ButtonState, Input::KM_Max>;
using GamepadStates = StaticArray<Input::GamepadState, Input::MaxSupportedControllers>;
using ActiveGamepads = StaticArray<bool, Input::MaxSupportedControllers>;

void InitializeInput(ApplicationInputMap& inputHandler);

bool IsPressed(Input::Buttons key);
bool IsDown(Input::Buttons key);
IntVector2 GetMousePosition();
}
