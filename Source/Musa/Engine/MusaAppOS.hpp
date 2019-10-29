#pragma once

#include "Types/Intrinsics.hpp"
#include "Types/UniquePtr.hpp"
#include "Input/WindowInputHandler.hpp"

class MusaApp;
class Window;

class MusaAppOS
{
public:
	MusaAppOS(UniquePtr<WindowInputHandler>&& inputHandler);
	virtual ~MusaAppOS() = default;

	virtual Window* CreateGameWindow(uint32 xPos, uint32 yPos, uint32 width, uint32 height) = 0;

	inline WindowInputHandler* GetInputHandler() { Assert(inputHandler); return inputHandler.Get(); }

protected:
	UniquePtr<WindowInputHandler> inputHandler;

};