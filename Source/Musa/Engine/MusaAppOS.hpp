// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "BasicTypes/UniquePtr.hpp"
#include "Input/WindowInputHandler.hpp"
#include "Math/Rect.hpp"

class MusaApp;
class Window;
struct IntVector2;

class MusaAppOS
{
public:
	MusaAppOS(UniquePtr<WindowInputHandler>&& inputHandler);
	virtual ~MusaAppOS() = default;

	virtual Window* CreateGameWindow(uint32 xPos, uint32 yPos, uint32 width, uint32 height) = 0;
	
	virtual void SetRawMouseInput(bool enabled, const Window& window) = 0;

	virtual void ShowCursor(bool showCursor) = 0;
	virtual void SetMousePosition(const IntVector2& mousePos) = 0;
	virtual IntVector2 GetMousePosition() const = 0;
	virtual void LockCursorToRect(const IntRect& rect) = 0;
	virtual void UnlockCursorFromRect() = 0;

	virtual void ProcessNativeGamepad() = 0;

	virtual void ProcessInputEvents() = 0;

	void PostProcessInputEvents();

	inline WindowInputHandler* GetInputHandler() { Assert(inputHandler.IsValid()); return inputHandler.Get(); }

protected:
	UniquePtr<WindowInputHandler> inputHandler;

};