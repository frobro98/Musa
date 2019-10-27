#pragma once

#include "Types/Intrinsics.hpp"
#include "Types/UniquePtr.hpp"
#include "Input/WindowInputHandler.hpp"

class MusaApp;
class Window;

class MusaAppOS
{
public:
	virtual ~MusaAppOS() = default;

	virtual Window* CreateGameWindow(uint32 xPos, uint32 yPos, uint32 width, uint32 height) = 0;

	inline void SetInputHandler(WindowInputHandler* handler) { inputHandler = handler; }

	inline WindowInputHandler* GetInputHandler() { Assert(inputHandler); return inputHandler; }

protected:
	WindowInputHandler* inputHandler = nullptr;

};