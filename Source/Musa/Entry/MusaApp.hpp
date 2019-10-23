#pragma once

#include "Types/UniquePtr.hpp"
#include "MusaEngine.hpp"
#include "Engine/MusaAppOS.hpp"
#include "Windowing/Window.h"
#include "UI/GameUIContext.hpp"

class MusaApp
{
public:
	MusaApp();

	void LaunchApplication();

	inline GameUIContext& GetUIContext() const { return *uiContext; }

private:


private:
	UniquePtr<MusaEngine> gameEngine;
	UniquePtr<GameUIContext> uiContext;
	UniquePtr<Window> appWindow;
	MusaAppOS* osApp;
};
