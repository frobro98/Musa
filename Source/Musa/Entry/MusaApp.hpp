#pragma once

#include "Types/UniquePtr.hpp"
#include "MusaEngine.hpp"
#include "Windowing/Window.h"
#include "UI/GameUIContext.hpp"

class MusaApp
{
public:
	MusaApp();

	void LaunchApplication();

	inline GameUIContext& GetUIContext() const { return *uiContext; }
	inline Window& GetWindow() const { return *appWindow; }

private:


private:
	UniquePtr<MusaEngine> gameEngine;
	UniquePtr<GameUIContext> uiContext;
	UniquePtr<Window> appWindow;
};
