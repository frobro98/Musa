#pragma once

#include "Input/IInputReceiver.hpp"
#include "Input/InputSettings.hpp"

class MusaEngine;
class MusaApp;

// TODO - Consider renaming this class. Possibilities are EngineInput or MusaInput
// TODO - Might not make sense to have the game input object conform to the input receiver. It might not need to know what happens when 
class GameInput final : public IInputReceiver
{
public:
	GameInput(MusaEngine& engine);

	inline InputSettings GetInputSettings() const { return inputSettings; }

	virtual InputEvents OnChar(tchar c, bool isRepeated) override;
	virtual InputEvents OnKeyUp(Inputs::Type input) override;
	virtual InputEvents OnKeyDown(Inputs::Type input, bool isRepeated) override;
	virtual InputEvents OnMouseUp(Inputs::Type input);
	virtual InputEvents OnMouseDown(Inputs::Type input);
	virtual InputEvents OnMouseMove(uint32 mouseX, uint32 mouseY) override;
	
	// These focus callbacks will refresh settings or disable settings depending on what settings exist
	virtual InputEvents OnFocusReceived() override;
	virtual InputEvents OnFocusLost() override;

	virtual void OnActivationChanged(bool activated) override;

	virtual IInputReceiver* GetParentInput() const override { return nullptr; }
	virtual IInputReceiver* GetChildInput() const override { return nullptr; }
	virtual IInputReceiver* GetSiblingInput() const override { return nullptr; }
	
	void SyncApplicationInput(MusaApp& app);

	void OnWindowClose();

	void LockCusorToView(bool shouldLock);
	void ShowCursor(bool shouldShow);

private:
	InputSettings inputSettings;
	MusaEngine& musaEngine;
	IntVector2 hiddenMousePosition;
	bool inputSettingsDirty;
};
