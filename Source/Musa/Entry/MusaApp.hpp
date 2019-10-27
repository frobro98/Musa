#pragma once

#include "Types/UniquePtr.hpp"
#include "Engine/MusaEngine.hpp"
#include "Engine/MusaAppOS.hpp"
#include "Window/Window.h"
#include "UI/GameUIContext.hpp"

class MusaApp
{
public:
	MusaApp();

	void LaunchApplication();

	inline GameUIContext& GetUIContext() const { return *uiContext; }

private:
	UniquePtr<MusaEngine> gameEngine;
	UniquePtr<GameUIContext> uiContext;
	UniquePtr<WindowInputHandler> inputHandler;
	UniquePtr<Window> appWindow;
	MusaAppOS* osApp;
};
