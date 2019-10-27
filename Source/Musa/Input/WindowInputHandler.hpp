#pragma once

#include "Input/Input.hpp"
#include "Containers/DynamicArray.hpp"

class IInputReceiver;
class MusaApp;

class WindowInputHandler final
{
public:
	WindowInputHandler(MusaApp& app);
	void HandleKeyUp(Inputs::Type input);
	void HandleKeyDown(Inputs::Type input, bool isRepeated);
	void HandleKeyChar(tchar c, bool isRepeated);
	void HandleMouseMove(uint32 mouseX, uint32 mouseY);

	void AddWindowInput(IInputReceiver* receiver);
	void RemoveWindowInput(IInputReceiver* receiver);

	inline void SetCurrentWindow(Window& win) { window = &win; }
	inline Window* GetWindow() { return window; }

private:
	DynamicArray<IInputReceiver*> inputReceivers;
	MusaApp& application;
	Window* window;
};
