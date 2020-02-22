#include "UIConsole.hpp"

namespace UI
{
Console::Console(Widget* parent)
	: Widget(parent),
	consoleInput(MakeUnique<TextInput>(this)),
	background(MakeUnique<Image>(this))
{
	textOnScreen = "_";
}

InputEvents Console::OnChar(tchar c, bool isRepeated)
{
	InputEvents events = consoleInput->OnChar(c, isRepeated);
	return events;
}

void Console::UpdateInternal()
{
	//consoleInput->Update(relativePosition, relativeScale);
	background->Update(relativePosition, relativeScale);
}

void Console::PrepareForRenderInternal()
{

}
}
