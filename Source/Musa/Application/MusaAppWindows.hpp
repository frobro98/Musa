// Copyright 2020, Nathan Blane

#pragma once

#include "Application/MusaAppOS.hpp"
#include "Containers/StaticArray.hpp"
#include "Input/Input.hpp"

class MusaAppWindows final : public MusaAppOS
{
public:
	MusaAppWindows(MusaApp& app);

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
	Input::GamepadStates controllers;
	Input::ActiveGamepads activeControllers;
	HINSTANCE instance;
};