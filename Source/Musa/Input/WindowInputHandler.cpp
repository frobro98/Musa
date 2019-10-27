
#include "WindowInputHandler.hpp"

WindowInputHandler::WindowInputHandler(MusaApp& app)
	: application(app)
{
}

void WindowInputHandler::HandleKeyUp(Inputs::Type input)
{
	UNUSED(input);
}

void WindowInputHandler::HandleKeyDown(Inputs::Type input, bool /*isRepeated*/)
{
	UNUSED(input);
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
