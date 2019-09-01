#include "InputManager.h"
#include "GameObject/GameObject.h"
#include "Windowing/Window.h"
#include "Containers/DynamicArray.hpp"
#include "Containers/Map.h"

namespace
{
struct Input
{
	using KeyPressedFunc = std::function<void()>;
	using KeyReleasedFunc = std::function<void()>;
	using KeyStateFunc = std::function<void(int32)>;

	struct KeyStateDesc
	{
		KeyStateFunc stateFunc;
		int32 stateModifier;
		int32 pad[1];
	};

	DynamicArray<KeyPressedFunc> keyPressedEvents;
	DynamicArray<KeyReleasedFunc> keyReleasedEvents;
	DynamicArray<KeyStateDesc> keyStateEvents;
	uint32 keyCode;
	uint32 endedDown : 1;
	uint32 previouslyDown : 1;

	Input()
		: endedDown(false),
		previouslyDown(false)
	{
	}
};

struct InputState
{
	DynamicArray<uint32> inputStateAccumulation[KeyState_Max];
	bool endedDown = false;
	bool previouslyDown = false;
	bool pad[2] = { false, false };
};

struct InputFireDescription
{
	using KeyPressedFunc = std::function<void()>;
	using KeyReleasedFunc = std::function<void()>;
	DynamicArray<KeyPressedFunc> keyPressedEvents;
	DynamicArray<KeyReleasedFunc> keyReleasedEvents;
};

struct InputHoldDescription
{
	using KeyStateFunc = std::function<void(int32)>;
	KeyStateFunc stateFunc;
	int32 stateModifier;
	int32 pad[1];
};

static Map<KeyInput, InputState> inputMap;
static Map<KeyInput, InputFireDescription> inputFireMap;
static Map<KeyInput, DynamicArray<InputHoldDescription>> inputHoldMap;

void FireKeyEvents(KeyInput key, InputState inputState)
{
	bool wasPressedNow = inputState.endedDown && !inputState.previouslyDown;
	bool wasReleasedNow = !inputState.endedDown && inputState.previouslyDown;
	InputFireDescription& keyFires = inputFireMap[key];

	if (wasPressedNow)
	{
		for (const auto& pressedFunc : keyFires.keyPressedEvents)
		{
			if (pressedFunc)
			{
				pressedFunc();
			}
		}
	}

	if (wasReleasedNow)
	{
		for (const auto& releasedFunc : keyFires.keyReleasedEvents)
		{
			if (releasedFunc)
			{
				releasedFunc();
			}
		}
	}
}

void ProcessKeyStates(KeyInput key, InputState inputState)
{
	DynamicArray<InputHoldDescription>& holdFunctions = inputHoldMap[key];
	bool isKeyDown = inputState.endedDown;
	for (const auto& holdFunc : holdFunctions)
	{
		holdFunc.stateFunc(isKeyDown ? holdFunc.stateModifier : 0);
	}
}

using MouseSubscriber = std::function<void(float, float)>;
DynamicArray<MouseSubscriber> mouseMovementSubscribers;

// Input inputs[static_cast<uint32>(KeyInput::Key_Max)];
// 
// Array<Input*> inputsToProcess;
// Array<uint32> inputsToRemove;

bool mouseMovementInitialized = false;
float prevMouseX = 0.f;
float prevMouseY = 0.f;

float currMouseX;
float currMouseY;


void MouseMovementInitialize()
{
	if (!mouseMovementInitialized)
	{
		POINT cursorPos;
		::GetCursorPos(&cursorPos);
		currMouseX = prevMouseX = static_cast<float>(cursorPos.x);
		currMouseY = prevMouseY = static_cast<float>(cursorPos.y);
		mouseMovementInitialized = true;
	}
}
}

namespace Internal
{
void KeyMessageDownReceived(uint32 keyCode, bool isPressed, bool /*isRepeated*/)
{
	KeyInput key = static_cast<KeyInput>(keyCode);
	InputState inputState;
	if (inputMap.TryFind(key, inputState))
	{
		inputState.endedDown = isPressed;
		inputMap[key] = inputState;
	}
}

void KeyMessageUpReceived(uint32 keyCode)
{
	KeyInput key = static_cast<KeyInput>(keyCode);
	InputState inputState;
	if (inputMap.TryFind(key, inputState))
	{
		inputState.endedDown = false;
		inputMap[key] = inputState;
	}
}

void MouseMovementChange()
{
	POINT cursorPos;
	::GetCursorPos(&cursorPos);
	currMouseX = static_cast<float>(cursorPos.x);
	currMouseY = static_cast<float>(cursorPos.y);
}
}

void InputManager::Initialize(Window& win)
{
	window = &win;
	MouseMovementInitialize();
}

bool InputManager::IsKeyPressed(KeyInput key) const
{
	bool result = false;
	InputState inputState;
	if (inputMap.TryFind(key, inputState))
	{
		result = inputState.endedDown && !inputState.previouslyDown;
	}
	return result;
}

void InputManager::Update()
{
	Instance().ProcessInputMessages();
	Instance().BroadcastInputMessages();
	Instance().ProcessMouseMovement();
}

void InputManager::SetContinuousInputFunction(KeyInput key, int32 stateMod, ContinuousInputFunction&& inputFunc)
{
	InputState inputState;
	if (!inputMap.TryFind(key, inputState))
	{
		inputMap.Add(key, InputState{});
		inputFireMap.Add(key, InputFireDescription{});
		inputHoldMap.Add(key, DynamicArray<InputHoldDescription>{});
	}

	InputHoldDescription desc = {};
	desc.stateFunc = std::move(inputFunc);
	desc.stateModifier = stateMod;
	inputHoldMap[key].Add(std::move(desc));
}

void InputManager::SetStateInputFunction(KeyInput key, KeyStateFlags state, StateChangedInputFunction&& inputFunc)
{
	InputState inputState;
	if (!inputMap.TryFind(key, inputState))
	{
		inputMap.Add(key, InputState{});
		inputFireMap.Add(key, InputFireDescription{});
		inputHoldMap.Add(key, DynamicArray<InputHoldDescription>{});
	}

	if (state == KeyState_Pressed)
	{
		inputFireMap[key].keyPressedEvents.Add(std::move(inputFunc));
	}
	else
	{
		inputFireMap[key].keyReleasedEvents.Add(std::move(inputFunc));
	}
}

void InputManager::SetMouseAxesFunction(MousePositionFunction&& mouseFunction)
{
	mouseMovementSubscribers.Add(std::move(mouseFunction));
}

void InputManager::ProcessInputMessages()
{
	MSG Message;
	while (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
}

void InputManager::ProcessMouseMovement()
{
	const float positionChangeX = Math::DegreesToRadians(currMouseX - prevMouseX);
	const float positionChangeY = Math::DegreesToRadians(currMouseY - prevMouseY);

	for (auto& func : mouseMovementSubscribers)
	{
		func(positionChangeX, positionChangeY);
	}

	extern bool showCursor;

	if (!showCursor)
	{
		RECT windowRect;
		GetWindowRect((HWND)window->GetWindowHandle(), &windowRect);
		int32 originX = windowRect.left;
		int32 originY = windowRect.top;

		int32 width = window->GetWidth();
		int32 height = window->GetHeight();
		int32 centerX = originX + (width / 2);
		int32 centerY = originY + (height / 2);
		SetCursorPos(centerX, centerY);
	}

// 	prevMouseX = currMouseX;
// 	prevMouseY = currMouseY;
}

void InputManager::BroadcastInputMessages()
{
	for (auto& keyInfoPair : inputMap)
	{
		KeyInput key = keyInfoPair.first;
		InputState& inputState = keyInfoPair.second;

		FireKeyEvents(key, inputState);

		ProcessKeyStates(key, inputState);

		inputState.previouslyDown = inputState.endedDown;
	}
}

InputManager& InputManager::Instance()
{
	static InputManager imInstance;
	return imInstance;
}

