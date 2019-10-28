
#include "WindowInputHandler.hpp"
#include "GameInput.hpp"

WindowInputHandler::WindowInputHandler(GameInput& input)
	: gameInput(input)
{
}

void WindowInputHandler::HandleKeyUp(Inputs::Type input)
{
	UNUSED(input);
}

void WindowInputHandler::HandleKeyDown(Inputs::Type input, bool isRepeated)
{
	gameInput.OnKeyDown(input, isRepeated);
}

void WindowInputHandler::HandleKeyChar(tchar c, bool /*isRepeated*/)
{
	UNUSED(c);
}

void WindowInputHandler::HandleMouseMove(uint32 mouseX, uint32 mouseY)
{
	UNUSED(mouseY, mouseX);
}

void WindowInputHandler::AddWindowInput(IInputReceiver* receiver)
{
	inputReceivers.Add(receiver);
}

void WindowInputHandler::RemoveWindowInput(IInputReceiver* receiver)
{
	inputReceivers.RemoveAll(receiver);
}
