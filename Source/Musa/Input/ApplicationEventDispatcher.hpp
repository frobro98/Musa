// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "Math/IntVector2.hpp"
#include "Input/InputDefinitions.hpp"
#include "UI/UIContext.hpp"
#include "Input/InputEvents.hpp"

class MusaApp;

namespace Input
{
struct ButtonState;
}

class ApplicationEventDispatcher
{
public:
	ApplicationEventDispatcher(MusaApp& app);

	void ClearCachedInputs();

	// Handle Application Events that then get dispatched if needed to the game
	void HandleKeyDown(Input::Buttons key, const Input::ButtonState& input, bool isRepeated);
	void HandleKeyUp(Input::Buttons key, const Input::ButtonState& input);
	void HandleKeyChar(tchar c, bool isRepeated);

	void HandleMouseDown(Input::Buttons mouse, const Input::ButtonState& inputState);
	void HandleMouseUp(Input::Buttons mouse, const Input::ButtonState& inputState);
	void HandleMouseMove(const IntVector2& mousePosition);
	void HandleRawMouseMove(const IntVector2& mousePosition, const IntVector2& deltaPosition);

	void HandleControllerButtonDown(u32 controllerIndex, Input::Buttons gamepadInput, const Input::ButtonState& state);
	void HandleControllerButtonUp(u32 controllerIndex, Input::Buttons gamepadInput, const Input::ButtonState& state);
	void HandleControllerAnalogChange(u32 controllerIndex, Input::Buttons gamepadInput, f32 normValue);

	void HandleWindowResizeEvent(const IntVector2& newWindowdimensions);
	void HandleWindowCloseEvent();
	void HandleWindowActivationChanged(bool activated);

	forceinline const DynamicArray<ButtonEvent>& GetButtonEvents() const { return buttonEvents; }
	forceinline const DynamicArray<MouseMoveEvent>& GetMouseMoveEvents() const { return mouseMoveEvents; }
	forceinline const DynamicArray<AnalogChangeEvent>& GetAnalogChangeEvents() const { return analogChangeEvents; }

private:
	void HandleInputEvents();

private:
	DynamicArray<ButtonEvent> buttonEvents;
	DynamicArray<MouseMoveEvent> mouseMoveEvents;
	DynamicArray<AnalogChangeEvent> analogChangeEvents;
	MusaApp& application;
	IntVector2 previousPosition;
};
