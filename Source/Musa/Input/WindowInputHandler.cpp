// Copyright 2020, Nathan Blane

#include "WindowInputHandler.hpp"
#include "Internal/InputInternal.hpp"
#include "GameInput.hpp"
#include "InputEvents.hpp"
#include "Window/Window.h"
#include "Entry/MusaApp.hpp"

#include "Debugging/DebugOutput.hpp"

WindowInputHandler::WindowInputHandler(MusaApp& app, GameInput& input)
	: gameInput(input),
	application(app)
{
}

void WindowInputHandler::HandleKeyUp(Inputs::Type input)
{
	Internal::KeyMessageUpReceived(input);

	InputEvents events = ProcessInputReceivers([&](IInputReceiver* receiver) 
	{
		Assert(receiver);
		return receiver->OnKeyUp(input);
	});

	if (!events.IsInputHandled())
	{
		events = gameInput.OnKeyUp(input);
		HandleInputEvents(events);
	}
}

void WindowInputHandler::HandleKeyDown(Inputs::Type input, bool isRepeated)
{
	Internal::KeyMessageDownReceived(input, true, isRepeated);

	InputEvents events = ProcessInputReceivers([&](IInputReceiver* receiver)
	{
		Assert(receiver);
		return receiver->OnKeyDown(input, isRepeated);
	});

	if (!events.IsInputHandled())
	{
		events = gameInput.OnKeyDown(input, isRepeated);
		HandleInputEvents(events);
	}
}

void WindowInputHandler::HandleMouseDown(Inputs::Type mouseButton)
{
	Internal::KeyMessageDownReceived(mouseButton, true, false);

	InputEvents events = ProcessInputReceivers([&](IInputReceiver* receiver)
	{
		Assert(receiver);
		return receiver->OnMouseDown(mouseButton);
	});

	if (!events.IsInputHandled())
	{
		events = gameInput.OnMouseDown(mouseButton);
		HandleInputEvents(events);
	}
}

void WindowInputHandler::HandleMouseUp(Inputs::Type mouseButton)
{
	Internal::KeyMessageUpReceived(mouseButton);

	InputEvents events = ProcessInputReceivers([&](IInputReceiver* receiver)
	{
		Assert(receiver);
		return receiver->OnMouseUp(mouseButton);
	});

	if (!events.IsInputHandled())
	{
		events = gameInput.OnMouseUp(mouseButton);
		HandleInputEvents(events);
	}
}

void WindowInputHandler::HandleKeyChar(tchar c, bool isRepeated)
{
	InputEvents events = ProcessInputReceivers([&](IInputReceiver* receiver)
	{
		Assert(receiver);
		return receiver->OnChar(c, isRepeated);
	});

	if (!events.IsInputHandled())
	{
		events = gameInput.OnChar(c, isRepeated);
		HandleInputEvents(events);
	}
}

void WindowInputHandler::HandleMouseMove(u32 mouseX, u32 mouseY)
{
	if (!rawInputTurnedOn)
	{
		IntVector2 mouseMovePos = IntVector2(mouseX, mouseY);
		currentMousePos = mouseMovePos;
		IntVector2 delta = currentMousePos - prevMousePos;

		if (currentMousePos != prevMousePos)
		{
			InputEvents events = ProcessInputReceivers([&](IInputReceiver* receiver)
			{
				Assert(receiver);
				return receiver->OnMouseMove(mouseMovePos, prevMousePos, delta);
			});

			if (!events.IsInputHandled())
			{
				events = gameInput.OnMouseMove(mouseMovePos, prevMousePos, delta);
				HandleInputEvents(events);
			}

			prevMousePos = mouseMovePos;
		}
	}
}

void WindowInputHandler::HandleRawMouseMove(u32 mouseX, u32 mouseY, i32 deltaX, i32 deltaY)
{
	IntVector2 delta(deltaX, deltaY);
	IntVector2 mouseMovePos(mouseX, mouseY);
	currentMousePos = mouseMovePos;

	if (deltaX || deltaY)
	{
		InputEvents events = ProcessInputReceivers([&](IInputReceiver* receiver)
		{
			Assert(receiver);
			return receiver->OnMouseMove(mouseMovePos, prevMousePos, delta);
		});

		if (!events.IsInputHandled())
		{
			events = gameInput.OnMouseMove(mouseMovePos, prevMousePos, delta);
			HandleInputEvents(events);
		}

		prevMousePos = mouseMovePos;
	}
}

void WindowInputHandler::HandleControllerAnalogChange(u32 controllerIndex, Inputs::Type analogType, f32 analogValue)
{
	Assert(analogType >= Inputs::Gamepad_LeftTrigger && Inputs::Gamepad_RightStick_YAxis);
	InputEvents events = ProcessInputReceivers([&](IInputReceiver* receiver)
	{
		Assert(receiver);
		return receiver->OnControllerAnalogChange(controllerIndex, analogType, analogValue);
	});

	if (!events.IsInputHandled())
	{
		events = gameInput.OnControllerAnalogChange(controllerIndex, analogType, analogValue);
		HandleInputEvents(events);
	}

}

void WindowInputHandler::HandleControllerButtonDown(u32 /*controllerIndex*/, Inputs::Type analogType)
{
	Internal::KeyMessageDownReceived(analogType, true, false);

	InputEvents events = ProcessInputReceivers([&](IInputReceiver* receiver)
	{
		Assert(receiver);
		return receiver->OnKeyDown(analogType, false);
	});

	if (!events.IsInputHandled())
	{
		events = gameInput.OnKeyDown(analogType, false);
		HandleInputEvents(events);
	}
}

void WindowInputHandler::HandleControllerButtonUp(u32 /*controllerIndex*/, Inputs::Type analogType)
{
	Internal::KeyMessageUpReceived(analogType);

	InputEvents events = ProcessInputReceivers([&](IInputReceiver* receiver)
	{
		Assert(receiver);
		return receiver->OnKeyUp(analogType);
	});

	if (!events.IsInputHandled())
	{
		events = gameInput.OnKeyUp(analogType);
		HandleInputEvents(events);
	}
}

void WindowInputHandler::HandleWindowResized(u32 newWidth, u32 newHeight)
{
	if (window)
	{
		window->Resize(newWidth, newHeight);
	}
}

void WindowInputHandler::HandleWindowClose()
{
	if (window)
	{
		window->Close();
		gameInput.OnWindowClose();
	}
}

void WindowInputHandler::HandleActivationChanged(bool activated)
{
	for (auto& receiver : inputReceivers)
	{
		receiver->OnActivationChanged(activated);
	}

	gameInput.OnActivationChanged(activated);
}

void WindowInputHandler::SetInputFocusToGame()
{
	InputEvents events = gameInput.OnFocusReceived();
	HandleInputEvents(events);
}

void WindowInputHandler::PostUpdateInput()
{
	gameInput.ProcessGameInputs();
}

void WindowInputHandler::AddInputStream(DynamicArray<IInputReceiver *>&& newInputStream)
{
	// NOTE - An input stream currently is representing a path of input within the UI tree
	inputReceivers = std::move(newInputStream);
}

void WindowInputHandler::AddInput(IInputReceiver& newInput)
{
	REF_CHECK(newInput);

	inputReceivers.Add(&newInput);
}

void WindowInputHandler::ClearInputStream()
{
	inputReceivers.Clear();
}

void WindowInputHandler::HandleInputEvents(const InputEvents& events)
{
	if (events.ContainsEvents())
	{
		std::optional<IntVector2> mousePos = events.GetChangedMousePosition();
		if (mousePos)
		{
			currentMousePos = *mousePos;
			prevMousePos = currentMousePos;
			application.SetMousePosition(mousePos.value());
		}

		std::optional<bool> lockCursor = events.GetLockCursor();
		if (lockCursor)
		{
			// TODO - Having lock and unlock as 2 separate calls might not make sense
			bool shouldLock = *lockCursor;
			if (shouldLock)
			{
				application.LockCursor();
			}
			else
			{
				application.UnlockCursor();
			}
		}

		std::optional<bool> showCursor = events.GetShowCursor();
		if (showCursor)
		{
			rawInputTurnedOn = !showCursor.value();
			application.ShowCursor(*showCursor);
			application.GetOSApp().SetRawMouseInput(rawInputTurnedOn, *window);
		}
	}
}
