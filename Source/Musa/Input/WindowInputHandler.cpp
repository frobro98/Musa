
#include "WindowInputHandler.hpp"
#include "GameInput.hpp"
#include "InputEvents.hpp"
#include "Window/Window.h"
#include "Entry/MusaApp.hpp"

WindowInputHandler::WindowInputHandler(MusaApp& app, GameInput& input)
	: gameInput(input),
	application(app)
{
}

void WindowInputHandler::HandleKeyUp(Inputs::Type input)
{
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

void WindowInputHandler::HandleMouseMove(uint32 mouseX, uint32 mouseY)
{
	IntVector2 mouseMovePos = IntVector2(mouseX, mouseY);
	currentMousePos = mouseMovePos;
	
	if (currentMousePos != prevMousePos)
	{
		InputEvents events = ProcessInputReceivers([&](IInputReceiver* receiver)
		{
			Assert(receiver);
			return receiver->OnMouseMove(mouseMovePos, prevMousePos);
		});

		if (!events.IsInputHandled())
		{
			events = gameInput.OnMouseMove(mouseMovePos, prevMousePos);
			HandleInputEvents(events);
		}

		prevMousePos = mouseMovePos;
	}
}

void WindowInputHandler::HandleWindowResized(uint32 newWidth, uint32 newHeight)
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
	// TODO - Finish having the same processing as the other handle inputs
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

void WindowInputHandler::AddWindowInput(IInputReceiver* receiver)
{
	inputReceivers.Add(receiver);
}

void WindowInputHandler::RemoveWindowInput(IInputReceiver* receiver)
{
	inputReceivers.RemoveAll(receiver);
}

void WindowInputHandler::HandleInputEvents(const InputEvents& events)
{
	if (events.ContainsEvents())
	{
		std::optional<IntVector2> mousePos = events.GetChangedMousePosition();
		if (mousePos)
		{
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
			application.ShowCursor(*showCursor);
		}
	}
}
