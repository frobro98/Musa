// Copyright 2020, Nathan Blane

#include "ApplicationEventRouter.hpp"
#include "Application/MusaApp.hpp"
#include "Input/InputEvents.hpp"

DEFINE_LOG_CHANNEL(KeyLogging);

void ApplicationEventRouter::HandleKeyDown(Input::Buttons key, const Input::DownState& input, bool isRepeated)
{
	Input::ButtonEvent event;
	event.button = key;
	event.buttonState = Input::ButtonState::Pressed;
	event.downState = input;
	event.isRepeated = isRepeated;
}

void ApplicationEventRouter::HandleKeyUp(Input::Buttons key, const Input::DownState& input)
{
	Input::ButtonEvent event;
	event.button = key;
	event.buttonState = Input::ButtonState::Released;
	event.downState = input;
	event.isRepeated = false;
}

void ApplicationEventRouter::HandleKeyChar(tchar c, bool isRepeated)
{
	UNUSED(c, isRepeated);
}

void ApplicationEventRouter::HandleMouseDown(Input::Buttons mouse, const Input::DownState& state)
{
	Input::ButtonEvent event;
	event.button = mouse;
	event.buttonState = Input::ButtonState::Pressed;
	event.downState = state;
	event.isRepeated = false;
}

void ApplicationEventRouter::HandleMouseUp(Input::Buttons mouse, const Input::DownState& state)
{
	Input::ButtonEvent event;
	event.button = mouse;
	event.buttonState = Input::ButtonState::Released;
	event.downState = state;
	event.isRepeated = false;
}

void ApplicationEventRouter::HandleMouseMove(const IntVector2& mousePosition)
{
	if (mousePosition != previousPosition)
	{
		Input::MouseEvent event;
		event.currentPosition = mousePosition;
		event.previousPosition = previousPosition;
		event.deltaPosition = mousePosition - previousPosition;

		previousPosition = mousePosition;
	}
}

void ApplicationEventRouter::HandleRawMouseMove(const IntVector2& mousePosition, const IntVector2& deltaPosition)
{
	if (mousePosition != previousPosition)
	{
		Input::MouseEvent event;
		event.currentPosition = mousePosition;
		event.previousPosition = previousPosition;
		event.deltaPosition = deltaPosition;

		previousPosition = mousePosition;
	}
}

void ApplicationEventRouter::HandleControllerButtonDown(u32 /*controllerIndex*/, Input::Buttons gamepadInput, const Input::DownState& state)
{
	Input::ButtonEvent event;
	event.button = gamepadInput;
	event.downState = state;
	event.isRepeated = false;
	event.buttonState = Input::ButtonState::Pressed;
}

void ApplicationEventRouter::HandleControllerButtonUp(u32 /*controllerIndex*/, Input::Buttons gamepadInput, const Input::DownState& state)
{
	Input::ButtonEvent event;
	event.button = gamepadInput;
	event.downState = state;
	event.isRepeated = false;
	event.buttonState = Input::ButtonState::Pressed;
}

void ApplicationEventRouter::HandleControllerAnalogChange(u32 /*controllerIndex*/, Input::Buttons gamepadInput, f32 normValue)
{
	Input::AnalogEvent event;
	event.analogButton = gamepadInput;
	event.normValue = normValue;
}

void ApplicationEventRouter::HandleWindowResizeEvent(const IntVector2& /*newWindowdimensions*/)
{
	//application.ResizeWindow(newWindowdimensions);

}

void ApplicationEventRouter::HandleWindowCloseEvent()
{
	//application.CloseWindow();
}

void ApplicationEventRouter::HandleWindowActivationChanged(bool /*activated*/)
{
	//application.Activation(activated);
}
