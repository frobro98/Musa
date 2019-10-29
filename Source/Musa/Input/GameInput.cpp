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

void GameInput::OnMouseUp(Inputs::Type input)
{
	UNUSED(input);
}

void GameInput::OnMouseDown(Inputs::Type input)
{
	UNUSED(input);
}

void GameInput::OnMouseMove(uint32 mouseX, uint32 mouseY)
{
	UNUSED(mouseX, mouseY);
}

void GameInput::OnFocusReceived()
{
}

void GameInput::OnFocusLost()
{
}

void GameInput::OnActivationChanged(bool activated)
{
	UNUSED(activated);
}

void GameInput::LockCusorToView(bool shouldLock)
{
	inputSettings.lockCursorToGame = shouldLock;
}

void GameInput::ShowCursor(bool shouldShow)
{
	inputSettings.cursorShown = shouldShow;
	inputSettings.limitMousePos = shouldShow;
}
