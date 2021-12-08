// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/UniquePtr.hpp"
#include "Application/MusaAppOS.hpp"
#include "Window/Window.h"
#include "UI/UIContext.hpp"
#include "Time/EngineTick.h"
#include "Input/ApplicationInputMap.hpp"
#include "Input/ApplicationEventRouter.hpp"
#include "MusaAPI.hpp"

class MUSA_API MusaApp
{
public:
	MusaApp();
	virtual ~MusaApp() {}

	void LaunchApplication();

	virtual void AppInit() = 0;
	virtual void AppLoop(f32 tick, const DynamicArray<Input::Event>& frameInputs) = 0;
	virtual void AppDeinit() = 0;

	forceinline ApplicationInputMap& GetInputMap() { return inputMap; }
	forceinline ApplicationEventRouter& GetInputRouter() { return *inputRouter; }

	// Application events
	void ResizeWindow(const IntVector2& newDimensions);
	void CloseWindow();
	void Activation(bool activated);

	void SetRawMouseInput(bool shouldEnable);
	void LockCursor();
	void UnlockCursor();
	void ShowCursor(bool showCursor);
	void SetMousePosition(const IntVector2& mousePos);
	// Gets the position of the cursor in the application window itself
	IntVector2 GetMousePosition() const;

protected:
	void InitializeOSInput();
	void InitializeApplicationWindow();
	void ProcessApplicationInputs();

protected:
	ApplicationInputMap inputMap;
	UniquePtr<UI::Context> uiContext;
	UniquePtr<Window> appWindow;
	UniquePtr<ApplicationEventRouter> inputRouter;
	UniquePtr<MusaAppOS> osApp;

private:
	EngineTick frameTick;
};
