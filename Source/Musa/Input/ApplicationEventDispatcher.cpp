// Copyright 2020, Nathan Blane

#include "ApplicationEventDispatcher.hpp"
#include "Entry/MusaApp.hpp"
#include "Input/InputEvents.hpp"

DEFINE_LOG_CHANNEL(KeyLogging);

ApplicationEventDispatcher::ApplicationEventDispatcher(MusaApp& app)
	: application(app)
{
}

void ApplicationEventDispatcher::HandleKeyDown(Input::Buttons key, const Input::ButtonState& input, bool isRepeated)
{
	if (key == Input::Key_Escape)
	{
		application.CloseWindow();
	}

	ButtonEvent event;
	event.button = key;
	event.state = input;
	event.isRepeated = isRepeated;

}

void ApplicationEventDispatcher::HandleKeyUp(Input::Buttons key, const Input::ButtonState& input)
{
	ButtonEvent event;
	event.button = key;
	event.state = input;
	event.isRepeated = false;

	// Push this event to UI/Application and then if those don't use up the input, push it to ECS world
}

void ApplicationEventDispatcher::HandleKeyChar(tchar c, bool isRepeated)
{
	UNUSED(c, isRepeated);
}

void ApplicationEventDispatcher::HandleMouseDown(Input::Buttons mouse, const Input::ButtonState& state)
{
	ButtonEvent event;
	event.button = mouse;
	event.state = state;
}

void ApplicationEventDispatcher::HandleMouseUp(Input::Buttons mouse, const Input::ButtonState& state)
{
	ButtonEvent event;
	event.button = mouse;
	event.state = state;
}

void ApplicationEventDispatcher::HandleMouseMove(const IntVector2& mousePosition)
{
	if (mousePosition != previousPosition)
	{
		MouseMoveEvent event;
		event.currentPosition = mousePosition;
		event.previousPosition = previousPosition;
		event.deltaPosition = mousePosition - previousPosition;

		previousPosition = mousePosition;
	}
}

void ApplicationEventDispatcher::HandleRawMouseMove(const IntVector2& mousePosition, const IntVector2& deltaPosition)
{
	if (mousePosition != previousPosition)
	{
		MouseMoveEvent event;
		event.currentPosition = mousePosition;
		event.previousPosition = previousPosition;
		event.deltaPosition = deltaPosition;

		previousPosition = mousePosition;
	}
}

void ApplicationEventDispatcher::HandleControllerButtonDown(u32 /*controllerIndex*/, Input::Buttons gamepadInput, const Input::ButtonState& state)
{
	ButtonEvent event;
	event.button = gamepadInput;
	event.state = state;
}

void ApplicationEventDispatcher::HandleControllerButtonUp(u32 /*controllerIndex*/, Input::Buttons gamepadInput, const Input::ButtonState& state)
{
	ButtonEvent event;
	event.button = gamepadInput;
	event.state = state;
}

void ApplicationEventDispatcher::HandleControllerAnalogChange(u32 /*controllerIndex*/, Input::Buttons gamepadInput, f32 normValue)
{
	AnalogChangeEvent event;
	event.analogButton = gamepadInput;
	event.normValue = normValue;
}

void ApplicationEventDispatcher::HandleWindowResizeEvent(const IntVector2& newWindowdimensions)
{
	application.ResizeWindow(newWindowdimensions);

	// Give event to UI and also game through ECS
}

void ApplicationEventDispatcher::HandleWindowCloseEvent()
{
	application.CloseWindow();

	// TODO - Give event to UI and also game through ECS
}

void ApplicationEventDispatcher::HandleWindowActivationChanged(bool activated)
{
	application.Activation(activated);
	// Give event to UI and also game through ECS
}
