// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/UniquePtr.hpp"
#include "Engine/MusaEngine.hpp"
#include "Application/MusaAppOS.hpp"
#include "Window/Window.h"
#include "UI/UIContext.hpp"
#include "Time/EngineTick.h"
#include "Input/ApplicationInputMap.hpp"
#include "Input/ApplicationEventDispatcher.hpp"

class MusaApp
{
public:
	MusaApp();
	virtual ~MusaApp() {}

	void LaunchApplication();

	forceinline MusaAppOS& GetOSApp() const { return *osApp; }
	forceinline ApplicationInputMap& GetInputMap() { return *inputMap; }
	forceinline ApplicationEventDispatcher& GetInputDispatcher() { return *inputDispatcher; }

	// Application events
	void ResizeWindow(const IntVector2& newDimensions);
	void CloseWindow();
	void Activation(bool activated);

	void LockCursor();
	void UnlockCursor();
	void ShowCursor(bool showCursor);
	void SetMousePosition(const IntVector2& mousePos);
	IntVector2 GetMousePosition() const;

	virtual void AppInit() = 0;
	virtual void AppLoop() = 0;
	virtual void AppDeinit() = 0;

private:
	void InitializeOSInput();
	void InitializeApplicationWindow();
	void SetupGameEngine();
	void TearDownGameEngine();
	void ProcessApplicationInputs();

protected:
	void ApplicationUpdate();

private:
	EngineTick frameTick;
	UniquePtr<MusaEngine> gameEngine;
	UniquePtr<UI::Context> uiContext;
	UniquePtr<Window> appWindow;
	UniquePtr<ApplicationInputMap> inputMap;
	UniquePtr<ApplicationEventDispatcher> inputDispatcher;
	MusaAppOS* osApp;
	bool isRunning = false;
};
