#include "GameInput.hpp"
#include "Engine/MusaEngine.hpp"

GameInput::GameInput(MusaEngine& engine)
	: musaEngine(engine)
{
}

void GameInput::OnKeyUp(Inputs::Type input)
{
	UNUSED(input);
}

void GameInput::OnKeyDown(Inputs::Type input, bool isRepeated)
{
	if (input == Inputs::Key_Escape)
	{
		musaEngine.StopEngine();
	}
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
