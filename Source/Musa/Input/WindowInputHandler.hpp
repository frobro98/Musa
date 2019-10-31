#pragma once

#include "Input/Input.hpp"
#include "Input/GameInput.hpp"
#include "Containers/DynamicArray.hpp"

class IInputReceiver;
class MusaApp;

class WindowInputHandler final
{
public:
	WindowInputHandler(MusaApp& app, GameInput& input);
	void HandleKeyUp(Inputs::Type input);
	void HandleKeyDown(Inputs::Type input, bool isRepeated);
	void HandleMouseDown(Inputs::Type mouseButton);
	void HandleMouseUp(Inputs::Type mouseButton);
	void HandleKeyChar(tchar c, bool isRepeated);
	void HandleMouseMove(uint32 mouseX, uint32 mouseY);

	void HandleWindowClose();
	void HandleActivationChanged(bool activated);

	void AddWindowInput(IInputReceiver* receiver);
	void RemoveWindowInput(IInputReceiver* receiver);

	inline void SetCurrentWindow(Window& win) { window = &win; }
	inline Window* GetWindow() { return window; }

private:
	void HandleInputEvents(const InputEvents& events);

private:
	DynamicArray<IInputReceiver*> inputReceivers;
	GameInput& gameInput;
	MusaApp& application;
	Window* window;
};
