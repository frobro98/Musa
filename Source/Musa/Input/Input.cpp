// Copyright 2020, Nathan Blane

#include "Input.hpp"
#include "Internal/InputInternal.hpp"
#include "Containers/StaticArray.hpp"
#include "Window/Window.h"
#include "Debugging/MetricInterface.hpp"

#include "Entry/MusaApp.hpp"

DECLARE_METRIC_GROUP(Input);
METRIC_STAT(UpdateInputs, Input);
METRIC_STAT(InputMapUpdate, Input);

struct InputState
{
	bool endedDown = false;
	bool previouslyDown = false;
};

static StaticArray<InputState, Inputs::Max> inputMap;
static MusaApp* application;

namespace Internal
{
void KeyMessageDownReceived(Inputs::Type key, bool isPressed, bool isRepeated)
{
	if (!isRepeated)
	{
		Assert(isPressed == true);
		inputMap[key].endedDown = isPressed;
		//inputManager.KeyDownReceived(key);
	}
}
void KeyMessageUpReceived(Inputs::Type key)
{
	inputMap[key].endedDown = false;
	//inputManager.KeyUpReceived(key);
}

void UpdateInputMap()
{
	BEGIN_TIMED_BLOCK(InputMapUpdate);
	for (uint32 i = 0; i < inputMap.Size(); ++i)
	{
		inputMap[i].previouslyDown = inputMap[i].endedDown;
	}
	END_TIMED_BLOCK(InputMapUpdate);
}
}

namespace Input
{
void InitializeInput(MusaApp& app)
{
	application = &app;
	Assert(application);

	ZeroMem(inputMap.internalData, sizeof(InputState) * inputMap.Size());
}

bool IsPressed(Inputs::Type key)
{
	const InputState& state = inputMap[key];
	return state.endedDown && !state.previouslyDown;
}

bool IsDown(Inputs::Type key)
{
	const InputState& state = inputMap[key];
	return state.endedDown;
}

IntVector2 GetMousePosition()
{
	return application->GetMousePosition();
}
}

