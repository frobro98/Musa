
#include "WindowInputHandler.hpp"
#include "GameInput.hpp"

WindowInputHandler::WindowInputHandler(MusaApp& app, GameInput& input)
	: gameInput(input),
	application(app)
{
}

void WindowInputHandler::HandleKeyUp(Inputs::Type input)
{
	for (auto& receiver : inputReceivers)
	{
		receiver->OnKeyUp(input);
	}

	gameInput.OnKeyUp(input);
}

void WindowInputHandler::HandleKeyDown(Inputs::Type input, bool isRepeated)
{
	for (auto& receiver : inputReceivers)
	{
		receiver->OnKeyDown(input, isRepeated);
	}

	gameInput.OnKeyDown(input, isRepeated);
}

void WindowInputHandler::HandleMouseDown(Inputs::Type mouseButton)
{
	for (auto& receiver : inputReceivers)
	{
		receiver->OnMouseDown(mouseButton);
	}

	gameInput.OnMouseDown(mouseButton);
}

void WindowInputHandler::HandleMouseUp(Inputs::Type mouseButton)
{
	for (auto& receiver : inputReceivers)
	{
		receiver->OnMouseUp(mouseButton);
	}

	gameInput.OnMouseUp(mouseButton);
}

void WindowInputHandler::HandleKeyChar(tchar c, bool isRepeated)
{
	for (auto& receiver : inputReceivers)
	{
		receiver->OnChar(c, isRepeated);
	}

	gameInput.OnChar(c, isRepeated);
}

void WindowInputHandler::HandleMouseMove(uint32 mouseX, uint32 mouseY)
{
	for (auto& receiver : inputReceivers)
	{
		receiver->OnMouseMove(mouseX, mouseY);
	}

	gameInput.OnMouseMove(mouseX, mouseY);
}

void WindowInputHandler::OnActivationChanged(bool activated)
{
	for (auto& receiver : inputReceivers)
	{
		receiver->OnActivationChanged(activated);
	}

	gameInput.OnActivationChanged(activated);
}

void WindowInputHandler::AddWindowInput(IInputReceiver* receiver)
{
	inputReceivers.Add(receiver);
}

void WindowInputHandler::RemoveWindowInput(IInputReceiver* receiver)
{
	inputReceivers.RemoveAll(receiver);
}
