#pragma once

#include "Engine/MusaAppOS.hpp"

class MusaAppWindows final : public MusaAppOS
{
public:
	MusaAppWindows(UniquePtr<WindowInputHandler>&& inputHandler);

	virtual Window* CreateGameWindow(uint32 xPos, uint32 yPos, uint32 width, uint32 height) override;

	virtual void SetMousePosition(const IntVector2& mousePos) override;
	virtual void LockCursorToRect(const IntRect& rect) override;
	virtual void UnlockCursorFromRect() override;


	virtual void ProcessInputEvents() override;

private:
	HINSTANCE instance;
};