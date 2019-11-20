#pragma once

#include "Input/IInputReceiver.hpp"
#include "Input/InputSettings.hpp"
#include "Input/PlayerInputContext.hpp"

class MusaEngine;
class MusaApp;

struct FrameInputs
{
	void Clear()
	{
		actions.Clear();
		ranges.Clear();
	}

	DynamicArray<const SingleInput*> actions;
	DynamicArray<const SingleInput*> states;
	DynamicArray<InputRangeValue> ranges;
};

// TODO - Consider renaming this class. Possibilities are EngineInput or MusaInput
// TODO - Might not make sense to have the game input object conform to the input receiver. It might not need to know what happens when 
class GameInput final : public IInputReceiver
{
public:
	using InputCallback = std::function<void(const FrameInputs&)>;

public:
	GameInput(MusaEngine& engine);

	inline InputSettings GetInputSettings() const { return inputSettings; }

	virtual InputEvents OnChar(tchar c, bool isRepeated) override;
	virtual InputEvents OnKeyUp(Inputs::Type input) override;
	virtual InputEvents OnKeyDown(Inputs::Type input, bool isRepeated) override;
	virtual InputEvents OnMouseUp(Inputs::Type input);
	virtual InputEvents OnMouseDown(Inputs::Type input);
	virtual InputEvents OnMouseMove(const IntVector2& currentMousePos, const IntVector2& prevMousePos, const IntVector2& delta) override;

	virtual InputEvents OnControllerAnalogChange(uint32 controllerIndex, Inputs::Type analogInput, float32 analogValue) override;
	
	// These focus callbacks will refresh settings or disable settings depending on what settings exist
	virtual InputEvents OnFocusReceived() override;
	virtual InputEvents OnFocusLost() override;

	virtual void OnActivationChanged(bool activated) override;

// 	virtual IInputReceiver* GetParentInput() const override { return nullptr; }
// 	virtual IInputReceiver* GetChildInput() const override { return nullptr; }

	void OnWindowClose();

	void LockCusorToView(bool shouldLock);
	void ShowCursor(bool shouldShow);

	// Game Input side of things
	void ProcessGameInputs();

	void RegisterInputCallback(InputCallback&& callback);
	void AddInputContext(const PlayerInputContext& context);
	void RemoveInputContext(StringView contextName);
	void PushInputContext(StringView contextName);
	void PopInputContext(StringView contextName);

private:
	void ClampInputToRangeAndStore(float32 value, const RangedInput& input);
	void NormalizeValueToRangeAndStore(float32 value, const RangedInput& input);

private:
	FrameInputs contextInputs;

	DynamicArray<InputCallback> callbacks;
	DynamicArray<PlayerInputContext> contexts;
	DynamicArray<uint32> activeContextIndices;

	InputSettings inputSettings;
	MusaEngine& musaEngine;
	IntVector2 mouseFrameMovement;
	IntVector2 controllerLeftStick;
	IntVector2 controllerRightStick;
	IntVector2 hiddenMousePosition;
	bool inputSettingsDirty;
};
