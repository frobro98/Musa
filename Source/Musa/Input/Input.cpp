
#include "Input.hpp"
#include "Internal/InputInternal.hpp"
#include "Containers/StaticArray.hpp"
#include "Utilities/CoreUtilities.hpp"
#include "Windowing/Window.h"
#include "Math/MathUtilities.h"
#include "Debugging/MetricInterface.hpp"

DECLARE_METRIC_GROUP(Input);
METRIC_STAT(UpdateInputs, Input);
METRIC_STAT(PumpMessages, Input);
METRIC_STAT(InputMapUpdate, Input);

struct InputState
{
	bool endedDown = false;
	bool previouslyDown = false;
};

static StaticArray<InputState, Inputs::Max> inputMap;

class InputManager
{
public:
	void Initialize(Window& /*win*/)
	{
		// TODO - Investigate bug with mouse initialization.
		// Bug consists of mouse moving in between initialization and update, so movement gets fucked up
		// This bug really only exists because the showing and hiding of the mouse is an incomplete implementation...
		POINT cursorPos;
		::GetCursorPos(&cursorPos);
		//::ScreenToClient((HWND)win.GetWindowHandle(), &cursorPos);
		currMouseX = prevMouseX = (float32)cursorPos.x;
		currMouseY = prevMouseY = (float32)cursorPos.y;
	}

	void SetupCursorReset(Window& win)
	{
		RECT windowRect;
		GetWindowRect((HWND)win.GetWindowHandle(), &windowRect);
		int32 originX = windowRect.left;
		int32 originY = windowRect.top;

		uint32 width = (uint32)win.GetWidth();
		uint32 height = (uint32)win.GetHeight();
		centerX = originX + (width / 2);
		centerY = originY + (height / 2);
	}

	void UpdateInputs()
	{
		SCOPED_TIMED_BLOCK(UpdateInputs);

// 		currMouseX = prevMouseX;
// 		currMouseY = prevMouseY;

		BEGIN_TIMED_BLOCK(InputMapUpdate);
		for (uint32 i = 0; i < inputMap.Size(); ++i)
		{
			inputMap[i].previouslyDown = inputMap[i].endedDown;
		}
		END_TIMED_BLOCK(InputMapUpdate);

		BEGIN_TIMED_BLOCK(PumpMessages);
		MSG Message;
		while (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		END_TIMED_BLOCK(PumpMessages);

		// Check every key to see if there are any that have either been updated or that are still down
		// For any that are like this, push a state change or a new value within a range or new action

		// These behaviors of the input will be completely based on the active contexts...
		// Action will only be registered if a "press" or "release" happens
		// State will continuously 

		for (auto& callback : callbacks)
		{
			callback(frameInputs);
		}

		frameInputs.actions.Clear();
		frameInputs.ranges.Clear();
	}

	void ResetCursor()
	{
		::SetCursorPos((uint32)prevMouseX, (uint32)prevMouseY);
	}

	void AddContext(const InputContext& context)
	{
		contexts.Add(context);
	}

	void PushActiveContext(StringView contextName)
	{
		InputContext* context = contexts.FindFirst([&contextName](const InputContext& c) { return c.contextName == contextName; });
		if (context)
		{
			activeContexts.Add(context);
		}
	}

	void PopActiveContext(StringView contextName)
	{
		InputContext** context = activeContexts.FindFirst([&contextName](const InputContext* c) { return c->contextName == contextName; });
		if (context)
		{
			activeContexts.RemoveAll(*context);
		}
	}

	void RegisterCallback(InputCallback&& callback)
	{
		callbacks.Add(std::move(callback));
	}

	// TODO - Figure out if we want to actually get rid of a callback...
	//void UnregisterCallback(InputCallback)

	void KeyDownReceived(Inputs::Type key)
	{
		for (const auto context : activeContexts)
		{
			if (int32 index = context->inputActions.FindFirstIndex([=](const SingleInput& i) {return i.input == key; }); index >= 0)
			{
				frameInputs.actions.Add(&context->inputActions[(uint32)index]);
				break;
			}
			if (int32 index = context->inputStates.FindFirstIndex([=](const SingleInput& i) {return i.input == key; }); index >= 0)
			{
				frameInputs.states.Add(&context->inputStates[(uint32)index]);
				break;
			}
		}
	}

	void KeyUpReceived(Inputs::Type key)
	{
		const SingleInput** input = frameInputs.states.FindFirst([=](const SingleInput* i) {return i->input == key; });
		if (input != nullptr)
		{
			frameInputs.states.RemoveAll(*input);
		}
	}

	void MouseMovementReceived(uint32 mousePosX, uint32 mousePosY)
	{
		currMouseX = (float32)mousePosX;
		currMouseY = (float32)mousePosY;
		float32 mousePosChangeX = currMouseX - prevMouseX;
		float32 mousePosChangeY = currMouseY - prevMouseY;
		for (const auto context : activeContexts)
		{
			if (int32 index = context->inputRanges.FindFirstIndex([=](const RangedInput& i) {return i.input.input == Inputs::Mouse_XAxis; }); index >= 0)
			{
				ClampInputToRangeAndStore((float32)mousePosChangeX, context->inputRanges[(uint32)index]);
				break;
			}
		}
		for (const auto context : activeContexts)
		{
			if (int32 index = context->inputRanges.FindFirstIndex([=](const RangedInput& i) {return i.input.input == Inputs::Mouse_YAxis; }); index >= 0)
			{
				ClampInputToRangeAndStore((float32)mousePosChangeY, context->inputRanges[(uint32)index]);
				break;
			}
		}
	}

	inline uint32 GetMouseCursorX() const { return (uint32)currMouseX; }
	inline uint32 GetMouseCursorY() const { return (uint32)currMouseY; }

private:
	void ClampInputToRangeAndStore(float32 value, const RangedInput& input)
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
			frameInputs.ranges.Add(inputValue);
		}
	}

private:
	FrameInputs frameInputs;
	// TODO - These should probably be sets not arrays
	DynamicArray<InputContext> contexts;
	DynamicArray<InputCallback> callbacks;
	DynamicArray<InputContext*> activeContexts;

	float32 currMouseX = 0, currMouseY = 0;
	float32 prevMouseX = 0, prevMouseY = 0;
	uint32 centerX = 0, centerY = 0;
};

namespace
{
InputManager inputManager;

static bool inputInitialized = false;
}

namespace Internal
{
void KeyMessageDownReceived(Inputs::Type key, bool isPressed, bool isRepeated)
{
	if (inputInitialized)
	{
		if (!isRepeated)
		{
			Assert(isPressed == true);
			inputMap[key].endedDown = isPressed;
			inputManager.KeyDownReceived(key);
		}
	}
}
void KeyMessageUpReceived(Inputs::Type key)
{
	if (inputInitialized)
	{
		inputMap[key].endedDown = false;
		inputManager.KeyUpReceived(key);
	}
}
void MouseMovementChange(uint32 mouseX, uint32 mouseY)
{
	if (inputInitialized)
	{
		inputManager.MouseMovementReceived(mouseX, mouseY);
	}
}
}

InputContext MakeInputContext(const StringView& name)
{
	InputContext context = {};
	context.contextName = String(*name, name.Length());
	context.nameHash = GetHash(name);
	return context;
}

void InitializeInput(Window& win)
{
	inputManager.Initialize(win);
	inputManager.SetupCursorReset(win);

	ZeroMem(inputMap.internalData, sizeof(InputState) * inputMap.Size());

	inputInitialized = true;
}

void InputUpdate()
{
	inputManager.UpdateInputs();
	inputManager.ResetCursor();
}

void AddInputCallback(InputCallback&& callback)
{
	inputManager.RegisterCallback(std::move(callback));
}

void AddInputContext(const InputContext& context)
{
	inputManager.AddContext(context);
}

void PushInputContext(StringView contextName)
{
	inputManager.PushActiveContext(contextName);
}

void RemoveInputContext(StringView contextName)
{
	inputManager.PopActiveContext(contextName);
}

bool IsInputPressed(Inputs::Type key)
{
	const InputState& state = inputMap[key];
	return state.endedDown && !state.previouslyDown;
}

bool IsInputDown(Inputs::Type key)
{
	const InputState& state = inputMap[key];
	return state.endedDown;
}

Vector2 GetMousePosition()
{
	uint32 currentMouseX = inputManager.GetMouseCursorX();
	uint32 currentMouseY = inputManager.GetMouseCursorY();
	return Vector2((float32)currentMouseX, (float32)currentMouseY);
}


