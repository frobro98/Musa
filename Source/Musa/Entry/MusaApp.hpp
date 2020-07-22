// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/UniquePtr.hpp"
#include "Engine/MusaEngine.hpp"
#include "Engine/MusaAppOS.hpp"
#include "Window/Window.h"
#include "UI/UIContext.hpp"
#include "Time/EngineTick.h"

class MusaApp
{
public:
	MusaApp();

	void LaunchApplication();

	inline UI::Context& GetUIContext() const { return *uiContext; }
	inline MusaAppOS& GetOSApp() const { return *osApp; }

	void LockCursor();
	void UnlockCursor();
	void ShowCursor(bool showCursor);
	void SetMousePosition(const IntVector2& mousePos);
	IntVector2 GetMousePosition() const;

private:
	void InitializeAppLogging();
	void InitializeOSInput();
	void InitializeApplicationWindow();
	void SetupGameEngine();
	void TearDownGameEngine();

	void ApplicationUpdate();

private:
	EngineTick frameTick;
	UniquePtr<MusaEngine> gameEngine;
	UniquePtr<UI::Context> uiContext;
	UniquePtr<Window> appWindow;
	MusaAppOS* osApp;
};
