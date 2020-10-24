// Copyright 2020, Nathan Blane

#pragma once

#include "Engine/MusaAppOS.hpp"
#include "Containers/StaticArray.hpp"

// TODO - Move this into a more input centric place...
constexpr u32 MaxSupportedControllers = 4;
constexpr u32 MaxSupportedControllerButtons = 12;

struct WindowsGamepadState
{
	// Store controller information unnormalized
	IntVector2 leftStick;
	IntVector2 rightStick;
	u8 leftTrigger;
	u8 rightTrigger;

	StaticArray<bool, MaxSupportedControllerButtons> buttonStates;
};

struct WindowsGamepadCollection
{
	StaticArray<WindowsGamepadState, MaxSupportedControllers> controllerStates;
	StaticArray<bool, MaxSupportedControllers> activeControllers;
};

class MusaAppWindows final : public MusaAppOS
{
public:
	MusaAppWindows(UniquePtr<WindowInputHandler>&& inputHandler);

	virtual Window* CreateGameWindow(u32 xPos, u32 yPos, u32 width, u32 height) override;

	virtual void SetRawMouseInput(bool enabled, const Window& window) override;

	virtual void ShowCursor(bool showCursor) override;
	virtual void SetMousePosition(const IntVector2& mousePos) override;
	virtual IntVector2 GetMousePosition() const override;
	virtual void LockCursorToRect(const Recti& rect) override;
	virtual void UnlockCursorFromRect() override;

	virtual void ProcessNativeGamepad() override;

	virtual void ProcessInputEvents() override;

private:
	//void ProcessAnalogControllerInputs(const XINPUT_GAMEPAD& xinputGamepad, WindowsGamepadState& state);

private:
	WindowsGamepadCollection controllers;
	HINSTANCE instance;
};