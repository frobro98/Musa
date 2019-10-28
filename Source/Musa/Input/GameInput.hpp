#pragma once

#include "Input/IInputReceiver.hpp"

class MusaEngine;

struct GameInputSettings
{

	bool lockCursorToGame;
	bool limitCursorToPosition;
};

class GameInput : public IInputReceiver
{
public:
	GameInput(MusaEngine& engine);

	inline GameInputSettings GetInputSettings() const { return inputSettings; }

	virtual void OnKeyUp(Inputs::Type input) override;
	virtual void OnKeyDown(Inputs::Type input, bool isRepeated) override;
	virtual void OnChar(tchar c, bool isRepeated) override;
	virtual void OnMouseMove(uint32 mouseX, uint32 mouseY) override;

private:
	MusaEngine& musaEngine;
	GameInputSettings inputSettings;
};
