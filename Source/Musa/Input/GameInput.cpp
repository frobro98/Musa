#include "GameInput.hpp"
#include "Engine/MusaEngine.hpp"
#include "Entry/MusaApp.hpp"
#include "Utilities/CoreUtilities.hpp"
#include "Math/MathUtilities.h"

GameInput::GameInput(MusaEngine& engine)
	: musaEngine(engine)
{
}

InputEvents GameInput::OnKeyUp(Inputs::Type input)
{
	const SingleInput** contextInput = contextInputs.states.FindFirst([=](const SingleInput* i) {return i->type == input; });
	if (contextInput != nullptr)
	{
		contextInputs.states.RemoveAll(*contextInput);
	}

	return InputEvents{};
}

InputEvents GameInput::OnKeyDown(Inputs::Type input, bool isRepeated)
{
	UNUSED(isRepeated);
	// TODO - This needs to be in a better place and also count for non-"shipping" builds
	if (input == Inputs::Key_Escape)
	{
		musaEngine.StopEngine();
	}
	else
	{
		for (auto index : activeContextIndices)
		{
			PlayerInputContext& context = contexts[index];
			if (int32 actionIndex = context.inputActions.FindFirstIndex([=](const SingleInput& i) {return i.type == input; }); actionIndex >= 0)
			{
				contextInputs.actions.AddUnique(&context.inputActions[(uint32)actionIndex]);
				break;
			}
			if (int32 stateIndex = context.inputStates.FindFirstIndex([=](const SingleInput& i) {return i.type == input; }); stateIndex >= 0)
			{
				contextInputs.states.AddUnique(&context.inputStates[(uint32)stateIndex]);
				break;
			}
		}
	}

	return InputEvents{};
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
	UNUSED(input);
	return InputEvents{};
}

InputEvents GameInput::OnMouseMove(const IntVector2& currentMousePos, const IntVector2& prevMousePos)
{
	IntVector2 diffPos = currentMousePos - prevMousePos;

	for (auto contextIndex : activeContextIndices)
	{
		PlayerInputContext& context = contexts[contextIndex];
		if (int32 index = context.inputRanges.FindFirstIndex([=](const RangedInput& ri) { return ri.input.type == Inputs::Mouse_XAxis; }); index >= 0)
		{
			mouseFrameMovement.x += diffPos.x;
			break;
		}
	}

	for (auto contextIndex : activeContextIndices)
	{
		PlayerInputContext& context = contexts[contextIndex];
		if (int32 index = context.inputRanges.FindFirstIndex([=](const RangedInput& ri) { return ri.input.type == Inputs::Mouse_YAxis; }); index >= 0)
		{
			mouseFrameMovement.y += diffPos.y;
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
		hiddenMousePosition = Input::GetMousePosition();
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
	inputSettings.limitMousePos = shouldShow;

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
		if (int32 index = context.inputRanges.FindFirstIndex([=](const RangedInput& i) {return i.input.type == Inputs::Mouse_XAxis; }); index >= 0)
		{
			ClampInputToRangeAndStore((float32)mouseFrameMovement.x, context.inputRanges[(uint32)index]);
			break;
		}
	}
	for (const auto contextIndex : activeContextIndices)
	{
		PlayerInputContext& context = contexts[contextIndex];
		if (int32 index = context.inputRanges.FindFirstIndex([=](const RangedInput& i) {return i.input.type == Inputs::Mouse_YAxis; }); index >= 0)
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
}

void GameInput::AddInputContext(const PlayerInputContext& context)
{
	contexts.AddUnique(context);
}

void GameInput::RemoveInputContext(StringView contextName)
{
	PlayerInputContext* context = contexts.FindFirst([&contextName](const PlayerInputContext& c) { return c.contextName == contextName; });
	if (context != nullptr)
	{
		contexts.RemoveFirst(*context);
	}
}

void GameInput::PushInputContext(StringView contextName)
{
	int32 index = contexts.FindFirstIndex([&contextName](const PlayerInputContext& c) { return c.contextName == contextName; });
	Assertf(index >= 0, "Trying to push an input context that doesn't exist");
	
	activeContextIndices.AddUnique((uint32)index);
}

void GameInput::PopInputContext(StringView contextName)
{
	int32 index = contexts.FindFirstIndex([&contextName](const PlayerInputContext& c) { return c.contextName == contextName; });
	Assertf(index >= 0, "Trying to pop an input context that doesn't exist");
	activeContextIndices.RemoveAll((uint32)index);
}

void GameInput::ClampInputToRangeAndStore(float32 value, const RangedInput & input)
{
	if (value > 0 || value < 0)
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
}