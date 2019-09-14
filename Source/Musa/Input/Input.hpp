#pragma once

#include "String/String.h"
#include "String/StringView.hpp"
#include "Containers/Map.h"
#include "InputDefinitions.hpp"
#include "Math/Vector2.hpp"

class InputManager;
class Window;

struct InputRange
{
	Vector2 clampRawRange;
	Vector2 normalizedRange;
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

struct InputContext
{
	String contextName;
	DynamicArray<SingleInput> inputActions;
	DynamicArray<SingleInput> inputStates;
	DynamicArray<RangedInput> axes;
	uint32 nameHash;
};

InputContext MakeContext(const String& name);

struct FrameInputs
{
	DynamicArray<SingleInput*> frameActions;
	DynamicArray<SingleInput*> frameStates;
	DynamicArray<RangedInput*> frameAxes;
};

using InputCallback = std::function<void(const FrameInputs&)>;

void InitializeInput(Window& win);
void AddCallback(InputCallback&& callback);
void AddInputContext(const InputContext& context);
void PushInputContext(StringView vs);
void RemoveInputContext(StringView vs);
bool IsInputPressed(Inputs::Type key);
Vector2 GetMousePosition();

InputManager& GetInputManager();