#pragma once

#include "String/String.h"
#include "String/StringView.hpp"
#include "Containers/Map.h"
#include "InputDefinitions.hpp"
#include "Math/IntVector2.hpp"

class InputManager;
class Window;

struct InputRange
{
	float32 minRawRange;
	float32 maxRawRange;
	float32 minNormalizedRange;
	float32 maxNormalizedRange;
};

struct SingleInput
{
	String inputName;
	Inputs::Type input;
};

struct RangedInput
{
	InputRange range;
	SingleInput input;
};

struct InputRangeValue
{
	const SingleInput* input;
	float32 rangeValue;
};

struct InputContext
{
	String contextName;
	DynamicArray<SingleInput> inputActions;
	DynamicArray<SingleInput> inputStates;
	DynamicArray<RangedInput> inputRanges;
	uint32 nameHash;
};

struct FrameInputs
{
	DynamicArray<const SingleInput*> actions;
	DynamicArray<const SingleInput*> states;
	DynamicArray<InputRangeValue> ranges;
};

InputContext MakeInputContext(const StringView& name);

using InputCallback = std::function<void(const FrameInputs&)>;

void InitializeInput();
void InputUpdate();

void AddInputCallback(InputCallback&& callback);
void AddInputContext(const InputContext& context);
void PushInputContext(StringView vs);
void RemoveInputContext(StringView vs);
bool IsInputPressed(Inputs::Type key);
bool IsInputDown(Inputs::Type key);
IntVector2 GetMousePosition();
