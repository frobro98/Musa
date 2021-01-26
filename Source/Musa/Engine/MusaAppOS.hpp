// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "BasicTypes/UniquePtr.hpp"
#include "Math/Rect.hpp"

class MusaApp;
class Window;
struct IntVector2;

class MusaAppOS
{
public:
	MusaAppOS(MusaApp& app);
	virtual ~MusaAppOS() = default;

	virtual Window* CreateGameWindow(u32 xPos, u32 yPos, u32 width, u32 height) = 0;
	
	virtual void SetRawMouseInput(bool enabled, const Window& window) = 0;

	virtual void ShowCursor(bool showCursor) = 0;
	virtual void SetMousePosition(const IntVector2& mousePos) = 0;
	virtual IntVector2 GetMousePosition() const = 0;
	virtual void LockCursorToRect(const Recti& rect) = 0;
	virtual void UnlockCursorFromRect() = 0;

	virtual void ProcessNativeGamepad() = 0;

	virtual void ProcessInputEvents() = 0;

	forceinline MusaApp& GetOwningApplication() { return owningApplication; }

protected:
	MusaApp& owningApplication;

};