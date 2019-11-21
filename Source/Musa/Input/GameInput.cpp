#include "GameInput.hpp"
#include "Engine/MusaEngine.hpp"
#include "Entry/MusaApp.hpp"
#include "Utilities/CoreUtilities.hpp"
#include "Math/MathUtilities.hpp"

GameInput::GameInput(MusaEngine& engine)
	: musaEngine(engine)
{
}

InputEvents GameInput::OnKeyUp(Inputs::Type input)
{
	const SingleInput** contextInput = contextInputs.states.FindFirstUsing([=](const SingleInput* i) {return i->type == input; });
	if (contextInput != nullptr)
	{
		contextInputs.states.RemoveAll(*contextInput);
	}

	return InputEvents{};
}

InputEvents GameInput::OnKeyDown(Inputs::Type input, bool isRepeated)
{
	InputEvents events = {};

	UNUSED(isRepeated);
	// TODO - This needs to be in a better place and also count for non-"shipping" builds
	if (input == Inputs::Key_Escape ||
		input == Inputs::Gamepad_StartButton)
	{
		events = InputEvents(Handled);
		musaEngine.StopEngine();
		return events;
	}
	if (input == Inputs::Key_J)
	{
		events = InputEvents(Handled);
		events.LockCursor(false);
		events.ShowCursor(true);
		inputSettings.limitMousePos = false;
	}
	if (input == Inputs::Key_I)
	{
		events = InputEvents(Handled);
		events.LockCursor(true);
		events.ShowCursor(false);
		inputSettings.limitMousePos = true;
	}
	

	{
		for (auto index : activeContextIndices)
		{
			PlayerInputContext& context = contexts[index];
			if (int32 actionIndex = context.inputActions.FindFirstIndexUsing([=](const SingleInput& i) {return i.type == input; }); actionIndex >= 0)
			{
				// TODO - This is sort of a hack to get at this information maybe. I'm not too sure if it is or not. If it is, input pressing might need to be done at a higher level?
				if (Input::IsPressed(input))
				{
					events = InputEvents(Handled);
					contextInputs.actions.AddUnique(&context.inputActions[(uint32)actionIndex]);
					break;
				}
			}
			if (int32 stateIndex = context.inputStates.FindFirstIndexUsing([=](const SingleInput& i) {return i.type == input; }); stateIndex >= 0)
			{
				events = InputEvents(Handled);
				contextInputs.states.AddUnique(&context.inputStates[(uint32)stateIndex]);
				break;
			}
		}
	}

	return events;
}

InputEvents GameInput::OnChar(tchar c, bool isRepeated)
{
	UNUSED(c, isRepeated);
	return InputEvents{};
}

InputEvents GameInput::OnMouseUp(Inputs::Type input)
{
	UNUSED(input);
	return InputEvents{};
}

InputEvents GameInput::OnMouseDown(Inputs::Type input)
{
	// TODO - This is where captures/focus come into play
	UNUSED(input);
	return InputEvents{};
}

InputEvents GameInput::OnMouseMove(const IntVector2& /*currentMousePos*/, const IntVector2& /*prevMousePos*/, const IntVector2& delta)
{
	for (auto contextIndex : activeContextIndices)
	{
		PlayerInputContext& context = contexts[contextIndex];
		if (int32 index = context.inputRanges.FindFirstIndexUsing([=](const RangedInput& ri) { return ri.input.type == Inputs::Mouse_XAxis; }); index >= 0)
		{
			mouseFrameMovement.x += delta.x;
			break;
		}
	}

	for (auto contextIndex : activeContextIndices)
	{
		PlayerInputContext& context = contexts[contextIndex];
		if (int32 index = context.inputRanges.FindFirstIndexUsing([=](const RangedInput& ri) { return ri.input.type == Inputs::Mouse_YAxis; }); index >= 0)
		{
			mouseFrameMovement.y += delta.y;
			break;
		}
	}

	InputEvents events;
	if (inputSettings.limitMousePos)
	{
		events.ChangeMousePosition(hiddenMousePosition);
	}

	return events;
}

InputEvents GameInput::OnControllerAnalogChange(uint32 /*controllerIndex*/, Inputs::Type analogInput, float32 analogValue)
{
	if (analogInput == Inputs::Gamepad_RightStick_YAxis)
	{
		analogValue = -analogValue;
	}

	for (const auto contextIndex : activeContextIndices)
	{
		PlayerInputContext& context = contexts[contextIndex];
		if (int32 index = context.inputRanges.FindFirstIndexUsing([=](const RangedInput& i) { return i.input.type == analogInput; }); index >= 0)
		{
			const RangedInput& rangedInput = context.inputRanges[(uint32)index];
			
			InputRangeValue value = {};
			value.input = &rangedInput.input;
			value.rangeValue = analogValue;
			contextInputs.ranges.Add(value);

			break;
		}
	}

	return InputEvents(Handled);
}

InputEvents GameInput::OnFocusReceived()
{
	// Game gets focus when:
	//	1) Game is created and there isn't anything in focus currently (Initial Focus)
	//	2) Game regains focus when UI has relinquished its focus and now there's nothing in focus (Gameplay input state change)

	InputEvents events;
	events.LockCursor(inputSettings.lockCursorToGame);
	events.ShowCursor(inputSettings.cursorShown);
	if (inputSettings.limitMousePos)
	{
		IntVector2 center = musaEngine.GetViewDimensions() / 2;
		hiddenMousePosition = center;
		events.ChangeMousePosition(hiddenMousePosition);
	}

	return events;
}

InputEvents GameInput::OnFocusLost()
{
	return InputEvents{};
}

void GameInput::OnActivationChanged(bool activated)
{
	UNUSED(activated);
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
	inputSettings.limitMousePos = !shouldShow;

	inputSettingsDirty = true;
}

void GameInput::RegisterInputCallback(InputCallback&& callback)
{
	callbacks.Add(std::move(callback));
}

void GameInput::ProcessGameInputs()
{
	for (const auto contextIndex : activeContextIndices)
	{
		PlayerInputContext& context = contexts[contextIndex];
		if (int32 index = context.inputRanges.FindFirstIndexUsing([=](const RangedInput& i) {return i.input.type == Inputs::Mouse_XAxis; }); index >= 0)
		{
			ClampInputToRangeAndStore((float32)mouseFrameMovement.x, context.inputRanges[(uint32)index]);
			break;
		}
	}
	for (const auto contextIndex : activeContextIndices)
	{
		PlayerInputContext& context = contexts[contextIndex];
		if (int32 index = context.inputRanges.FindFirstIndexUsing([=](const RangedInput& i) {return i.input.type == Inputs::Mouse_YAxis; }); index >= 0)
		{
			ClampInputToRangeAndStore((float32)mouseFrameMovement.y, context.inputRanges[(uint32)index]);
			break;
		}
	}

	
	// Fill up frame input
	for (auto& callback : callbacks)
	{
		callback(contextInputs);
	}

	contextInputs.Clear();

	mouseFrameMovement.x = 0;
	mouseFrameMovement.y = 0;
}

void GameInput::AddInputContext(const PlayerInputContext& context)
{
	contexts.AddUnique(context);
}

void GameInput::RemoveInputContext(StringView contextName)
{
	PlayerInputContext* context = contexts.FindFirstUsing([&contextName](const PlayerInputContext& c) { return c.contextName == contextName; });
	if (context != nullptr)
	{
		contexts.RemoveFirst(*context);
	}
}

void GameInput::PushInputContext(StringView contextName)
{
	int32 index = contexts.FindFirstIndexUsing([&contextName](const PlayerInputContext& c) { return c.contextName == contextName; });
	Assertf(index >= 0, "Trying to push an input context that doesn't exist");
	
	activeContextIndices.AddUnique((uint32)index);
}

void GameInput::PopInputContext(StringView contextName)
{
	int32 index = contexts.FindFirstIndexUsing([&contextName](const PlayerInputContext& c) { return c.contextName == contextName; });
	Assertf(index >= 0, "Trying to pop an input context that doesn't exist");
	activeContextIndices.RemoveAll((uint32)index);
}

void GameInput::ClampInputToRangeAndStore(float32 value, const RangedInput& input)
{
	const InputRange& range = input.range;
	value = Clamp(value, range.minRawRange, range.maxRawRange);

	float32 lerpT = (value - range.minRawRange) / (range.maxRawRange - range.minRawRange);
	float32 normValue = Math::Lerp(range.minNormalizedRange, range.maxNormalizedRange, lerpT);// (lerpT * (range.maxNormalizedRange - range.minNormalizedRange)) + range.minNormalizedRange;
	InputRangeValue inputValue = {};
	inputValue.input = &input.input;
	inputValue.rangeValue = normValue;
	contextInputs.ranges.Add(inputValue);
}

// TODO - This function should be expanded to be a utility
void GameInput::NormalizeValueToRangeAndStore(float32 normValue, const RangedInput& input)
{
	Assert(normValue >= 0.f && normValue <= 1.f);
	float32 retVal;
	const InputRange& range = input.range;
	if (Math::IsZero(normValue))
	{
		retVal = range.minRawRange;
	}
	else
	{
		constexpr float32 oldMin = 0;
		constexpr float32 oldMax = 1;
		constexpr float32 oldRange = oldMax - oldMin;
		const float32 newRange = range.maxRawRange - range.minRawRange;
		retVal = (((normValue - oldMin) * newRange) / oldRange) + range.minRawRange;
	}

	InputRangeValue inputValue = {};
	inputValue.input = &input.input;
	inputValue.rangeValue = retVal;
	contextInputs.ranges.Add(inputValue);
}
