#include "GameInput.hpp"

void GameInput::OnKeyUp(Inputs::Type input)
{
	UNUSED(input);
}

void GameInput::OnKeyDown(Inputs::Type input, bool isRepeated)
{
	UNUSED(input, isRepeated);
}

void GameInput::OnChar(tchar c, bool isRepeated)
{
	UNUSED(c, isRepeated);
}

void GameInput::OnMouseMove(uint32 mouseX, uint32 mouseY)
{
	UNUSED(mouseX, mouseY);
}
