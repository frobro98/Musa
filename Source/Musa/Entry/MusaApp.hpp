#pragma once

#include "Types/UniquePtr.hpp"
#include "Engine/MusaEngine.hpp"
#include "Engine/MusaAppOS.hpp"
#include "Window/Window.h"
#include "UI/GameUIContext.hpp"
#include "Time/EngineTick.h"

class MusaApp
{
public:
	MusaApp();

	void LaunchApplication();

	inline GameUIContext& GetUIContext() const { return *uiContext; }

	void LockCursor();
	void UnlockCursor();
	void ShowCursor(bool showCursor);
	void SetMousePosition(const IntVector2& mousePos);

private:
	void InitializeOSInput();
	void InitializeApplicationWindow();
	void SetupGameEngine();

	void ApplicationUpdate();

private:
	EngineTick frameTick;
	UniquePtr<MusaEngine> gameEngine;
	UniquePtr<GameUIContext> uiContext;
	UniquePtr<Window> appWindow;
	MusaAppOS* osApp;
};
