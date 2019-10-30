#pragma once

#include "Input/IInputReceiver.hpp"

class MusaEngine;

struct GameInputSettings
{
	bool lockCursorToGame;
	bool cursorShown;
	bool limitMousePos; // True when the cursor is hidden. TODO - Consider why this is and maybe have it be independent of when the cursor is shown
};

// TODO - Might not make sense to have the game input object conform to the input receiver. It might not need to know what happens when 
class GameInput : public IInputReceiver
{
public:
	GameInput(MusaEngine& engine);

	inline GameInputSettings GetInputSettings() const { return inputSettings; }

	virtual void OnKeyUp(Inputs::Type input) override;
	virtual void OnKeyDown(Inputs::Type input, bool isRepeated) override;
	virtual void OnChar(tchar c, bool isRepeated) override;
	virtual void OnMouseUp(Inputs::Type input);
	virtual void OnMouseDown(Inputs::Type input);
	virtual void OnMouseMove(uint32 mouseX, uint32 mouseY) override;
	
	// These focus callbacks will refresh settings or disable settings depending on what settings exist
	virtual void OnFocusReceived() override;
	virtual void OnFocusLost() override;

	virtual void OnActivationChanged(bool activated) override;

	virtual IInputReceiver* GetParentInput() const override { return nullptr; }
	virtual IInputReceiver* GetChildInput() const override { return nullptr; }
	virtual IInputReceiver* GetSiblingInput() const override { return nullptr; }

	void OnWindowClose();

	void LockCusorToView(bool shouldLock);
	void ShowCursor(bool shouldShow);

private:
	GameInputSettings inputSettings;
	MusaEngine& musaEngine;
};
