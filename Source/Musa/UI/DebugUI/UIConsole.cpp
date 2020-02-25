#include "UIConsole.hpp"

namespace UI
{
Console::Console()
	: background(MakeUnique<Image>(this))
{
}
Console::Console(Widget* parent)
	: Widget(parent),
	//consoleInput(MakeUnique<TextInput>(this)),
	background(MakeUnique<Image>(this))
{
	textOnScreen = "_";
}

InputEvents Console::OnKeyDown(Inputs::Type keyType, bool /*isRepeated*/)
{
	// Handling delete/backspace/tab/arrow key movements

	if (keyType == Inputs::Key_Tilde)
	{

	}

	return InputEvents();
}

InputEvents Console::OnChar(tchar c, bool /*isRepeated*/)
{
	constexpr tchar minChar = 32;
	constexpr tchar maxChar = 127;
	if (c >= minChar && c < maxChar)
	{
		return InputEvents(Handled);
	}
	//InputEvents events = consoleInput->OnChar(c, isRepeated);
	return InputEvents();
}

void Console::UpdateInternal()
{
	//consoleInput->Update(relativePosition, relativeScale);
	background->Update(relativePosition, relativeScale);
}

void Console::PrepareForRenderInternal(WidgetBatchElements& widgetElements)
{
	UNUSED(widgetElements);
}
}
