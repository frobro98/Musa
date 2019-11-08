#pragma once

#include "Engine/MusaAppOS.hpp"

class MusaAppWindows final : public MusaAppOS
{
public:
	MusaAppWindows(UniquePtr<WindowInputHandler>&& inputHandler);

	virtual Window* CreateGameWindow(uint32 xPos, uint32 yPos, uint32 width, uint32 height) override;

	virtual void SetRawMouseInput(bool enabled, const Window& window) override;

	virtual void ShowCursor(bool showCursor) override;
	virtual void SetMousePosition(const IntVector2& mousePos) override;
	virtual IntVector2 GetMousePosition() const override;
	virtual void LockCursorToRect(const IntRect& rect) override;
	virtual void UnlockCursorFromRect() override;


	virtual void ProcessInputEvents() override;

	void PostProcessInputEvents();

private:
	HINSTANCE instance;
};