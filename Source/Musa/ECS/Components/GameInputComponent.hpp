// Copyright 2020, Nathan Blane

#pragma once

#include "ECS/Component.hpp"

enum class InputEventType
{
	Pressed,
	Repeated,
	Released
};

struct InputAction
{
	Input::Buttons input;
	InputEventType type;
};

struct InputState
{
	Input::Buttons input;
	f32 value;
};

struct GameInputComponent : Musa::Component
{
	// Actions
	DynamicArray<InputAction> inputActions;
	// States, with values within a specific range
	DynamicArray<InputState> inputStates;
};
