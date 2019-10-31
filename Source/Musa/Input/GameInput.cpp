#include "GameInput.hpp"
#include "Engine/MusaEngine.hpp"

GameInput::GameInput(MusaEngine& engine)
	: musaEngine(engine)
{
}

InputEvents GameInput::OnKeyUp(Inputs::Type input)
{
	UNUSED(input);
	return InputEvents();
}

InputEvents GameInput::OnKeyDown(Inputs::Type input, bool isRepeated)
{
	if (input == Inputs::Key_Escape)
	{
		musaEngine.StopEngine();
	}
	UNUSED(input, isRepeated);
	return InputEvents();
}

InputEvents GameInput::OnChar(tchar c, bool isRepeated)
{
	UNUSED(c, isRepeated);
	return InputEvents();
}

InputEvents GameInput::OnMouseUp(Inputs::Type input)
{
	UNUSED(input);
	return InputEvents();
}

InputEvents GameInput::OnMouseDown(Inputs::Type input)
{
	UNUSED(input);
	return InputEvents();
}

InputEvents GameInput::OnMouseMove(uint32 mouseX, uint32 mouseY)
{
	UNUSED(mouseX, mouseY);
	return InputEvents();
}

InputEvents GameInput::OnFocusReceived()
{
	// Game gets focus when:
	//	1) Game is created and there isn't anything in focus currently (Initial Focus)
	//	2) Game regains focus when UI has relinquished its focus and now there's nothing in focus (Gameplay input state change)
	return InputEvents();
}

InputEvents GameInput::OnFocusLost()
{
	return InputEvents();
}

void GameInput::OnActivationChanged(bool activated)
{
	UNUSED(activated);
}

void GameInput::SyncApplicationInput(MusaApp& app)
{
	// TODO - This behavior should ALL be handled when the game input gains focus
	if (inputSettingsDirty)
	{
		UNUSED(app);
	}
}

void GameInput::OnWindowClose()
{
	musaEngine.StopEngine();
}

void GameInput::LockCusorToView(bool shouldLock)
{
	inputSettings.lockCursorToGame = shouldLock;

	inputSettingsDirty = true;
}

void GameInput::ShowCursor(bool shouldShow)
{
	inputSettings.cursorShown = shouldShow;
	inputSettings.limitMousePos = shouldShow;

	inputSettingsDirty = true;
}
