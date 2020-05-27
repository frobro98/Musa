// Copyright 2020, Nathan Blane

#pragma once

#include "Input/Input.hpp"
#include "Input/GameInput.hpp"
#include "Containers/DynamicArray.hpp"

class IInputReceiver;
class MusaApp;
class Window;

class WindowInputHandler final
{
public:
	WindowInputHandler(MusaApp& app, GameInput& input);
	void HandleKeyUp(Inputs::Type input);
	void HandleKeyDown(Inputs::Type input, bool isRepeated);
	void HandleMouseDown(Inputs::Type mouseButton);
	void HandleMouseUp(Inputs::Type mouseButton);
	void HandleKeyChar(tchar c, bool isRepeated);
	void HandleMouseMove(u32 mouseX, u32 mouseY);
	void HandleRawMouseMove(u32 mouseX, u32 mouseY, i32 deltaX, i32 deltaY);
	void HandleControllerAnalogChange(u32 controllerIndex, Inputs::Type analogType, f32 analogValue);
	void HandleControllerButtonDown(u32 controllerIndex, Inputs::Type analogType);
	void HandleControllerButtonUp(u32 controllerIndex, Inputs::Type analogType);

	void HandleWindowResized(u32 newWidth, u32 newHeight);
	void HandleWindowClose();
	void HandleActivationChanged(bool activated);

	void SetInputFocusToGame();

	void PostUpdateInput();

	// TODO - Does input stream correctly describe behavior of essentially an input path?
	// Answer: No
	void AddInputStream(DynamicArray<IInputReceiver*>&& newInputStream);
	void AddInput(IInputReceiver& newInput);
	void ClearInputStream();

	inline void SetCurrentWindow(Window& win) { window = &win; }
	inline Window* GetWindow() { return window; }

private:
	template <typename Func>
	InputEvents ProcessInputReceivers(Func&& receiverFunc)
	{
		InputEvents events;
		for(i32 i = (i32)inputReceivers.Size() - 1; i >= 0 && !events.IsInputHandled(); --i)
		{
			events = receiverFunc(inputReceivers[i]);
			HandleInputEvents(events);
		}

		return events;
	}

	void HandleInputEvents(const InputEvents& events);

private:
	IntVector2 currentMousePos;
	IntVector2 prevMousePos;
	DynamicArray<IInputReceiver*> inputReceivers;
	GameInput& gameInput;
	MusaApp& application;
	Window* window;
	bool rawInputTurnedOn = false;
};
