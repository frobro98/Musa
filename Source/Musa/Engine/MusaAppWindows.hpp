// Copyright 2020, Nathan Blane

#pragma once

#include "Engine/MusaAppOS.hpp"
#include "Containers/StaticArray.hpp"

// TODO - Move this into a more input centric place...
constexpr uint32 MaxSupportedControllers = 4;
constexpr uint32 MaxSupportedControllerButtons = 12;

struct WindowsGamepadState
{
	// Store controller information unnormalized
	IntVector2 leftStick;
	IntVector2 rightStick;
	uint8 leftTrigger;
	uint8 rightTrigger;

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

	virtual Window* CreateGameWindow(uint32 xPos, uint32 yPos, uint32 width, uint32 height) override;

	virtual void SetRawMouseInput(bool enabled, const Window& window) override;

	virtual void ShowCursor(bool showCursor) override;
	virtual void SetMousePosition(const IntVector2& mousePos) override;
	virtual IntVector2 GetMousePosition() const override;
	virtual void LockCursorToRect(const IntRect& rect) override;
	virtual void UnlockCursorFromRect() override;

	virtual void ProcessNativeGamepad() override;

	virtual void ProcessInputEvents() override;

private:
	//void ProcessAnalogControllerInputs(const XINPUT_GAMEPAD& xinputGamepad, WindowsGamepadState& state);

private:
	WindowsGamepadCollection controllers;
	HINSTANCE instance;
};