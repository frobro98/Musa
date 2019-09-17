
#include "Input.hpp"
#include "Input/InputManager.h"
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
// Could theoretically live in a vector for ints and not for floats???
static uint32 currentMouseX = 0;
static uint32 currentMouseY = 0;

class InputMan
{
public:
	void Initialize(Window& win)
	{
		// TODO - Investigate bug with mouse initialization.
		// Bug consists of mouse moving in between initialization and update, so movement gets fucked up
		// This bug really only exists because the showing and hiding of the mouse is an incomplete implementation...
		POINT cursorPos;
		::GetCursorPos(&cursorPos);
		::ScreenToClient((HWND)win.GetWindowHandle(), &cursorPos);
		currMouseX = prevMouseX = (float32)cursorPos.x;
		currMouseY = prevMouseY = (float32)cursorPos.y;
		
		RECT windowRect;
		GetWindowRect((HWND)win.GetWindowHandle(), &windowRect);
		int32 originX = windowRect.left;
		int32 originY = windowRect.top;

		int32 width = win.GetWidth();
		int32 height = win.GetHeight();
		centerX = originX + (width / 2);
		centerY = originY + (height / 2);
	}

	void UpdateInputs()
	{
		SCOPED_TIMED_BLOCK(UpdateInputs);
		// Check every key to see if there are any that have either been updated or that are still down
		// For any that are like this, push a state change or a new value within a range or new action

		// These behaviors of the input will be completely based on the active contexts...
		// Action will only be registered if a "press" or "release" happens
		// State will continuously 

		for (auto& callback : callbacks)
		{
			callback(frameInputs);
		}

		BEGIN_TIMED_BLOCK(InputMapUpdate);
		for (uint32 i = 0; i < inputMap.Size(); ++i)
		{
			inputMap[i].previouslyDown = inputMap[i].endedDown;
		}
		END_TIMED_BLOCK(InputMapUpdate);

		frameInputs.actions.Clear();
		frameInputs.ranges.Clear();

		SetCursorPos(centerX, centerY);
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
				ClampInputToRangeAndStore(mousePosChangeX, context->inputRanges[(uint32)index]);
				break;
			}
		}
		for (const auto context : activeContexts)
		{
			if (int32 index = context->inputRanges.FindFirstIndex([=](const RangedInput& i) {return i.input.input == Inputs::Mouse_YAxis; }); index >= 0)
			{
				ClampInputToRangeAndStore(mousePosChangeY, context->inputRanges[(uint32)index]);
				break;
			}
		}

// 		prevMouseX = currMouseX;
// 		prevMouseY = currMouseY;
	}

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

	void MouseMovementInitialize()
	{
		POINT cursorPos;
		::GetCursorPos(&cursorPos);
		currMouseX = prevMouseX = (float32)cursorPos.x;
		currMouseY = prevMouseY = (float32)cursorPos.y;
	}

private:
	FrameInputs frameInputs;
	// TODO - These should probably be sets not arrays
	DynamicArray<InputContext> contexts;
	DynamicArray<InputCallback> callbacks;
	DynamicArray<InputContext*> activeContexts;

	float32 currMouseX = 0, currMouseY = 0;
	float32 prevMouseX = 0, prevMouseY = 0;
	int32 centerX = 0, centerY = 0;
};

namespace
{
InputMan inputManager;
}

namespace Internal
{
void KeyMessageDownReceived(Inputs::Type key, bool isPressed, bool /*isRepeated*/)
{
	Assert(isPressed == true);
	inputMap[key].endedDown = isPressed;
	inputManager.KeyDownReceived(key);
}
void KeyMessageUpReceived(Inputs::Type key)
{
	inputMap[key].endedDown = false;
	inputManager.KeyUpReceived(key);
}
void MouseMovementChange(uint32 mouseX, uint32 mouseY)
{
	currentMouseX = mouseX;
	currentMouseY = mouseY;
	inputManager.MouseMovementReceived(mouseX, mouseY);
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
	ZeroMem(inputMap.internalData, sizeof(InputState) * inputMap.Size());
}

void InputUpdate()
{
	BEGIN_TIMED_BLOCK(PumpMessages);
	MSG Message;
	while (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	END_TIMED_BLOCK(PumpMessages);
	inputManager.UpdateInputs();
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

bool IsInputPressed(Inputs::Type /*key*/)
{
	return false;
}

Vector2 GetMousePosition()
{
	return Vector2();
}

InputManager& GetInputManager()
{
	return InputManager::Instance();
}

