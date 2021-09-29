// Copyright 2020, Nathan Blane

#include "MusaApp.hpp"
#include "Musa.hpp"
#include "MusaAppWindows.hpp"
#include "File/DirectoryLocations.hpp"
#include "Engine/FrameData.hpp"
#include "Engine/Internal/FrameDataInternal.hpp"
#include "Input/Internal/InputInternal.hpp"
#include "Graphics/GraphicsInterface.hpp"
#include "Logging/LogFunctions.hpp"
#include "Logging/Sinks/ConsoleWindowSink.hpp"
#include "Logging/Sinks/DebugOutputWindowSink.hpp"
#include "Logging/Sinks/LogFileSink.hpp"

DEFINE_LOG_CHANNEL(AppLog);

constexpr i32 windowWidth = 1080;
constexpr i32 windowHeight = 720;

MusaApp::MusaApp()
{
	uiContext = MakeUnique<UI::Context>(windowWidth, windowHeight);
}

void MusaApp::LaunchApplication()
{
	AppInit();

	InitializeOSInput();
	InitializeApplicationWindow();

	SetupGameEngine();

	AppLoop();

	// TODO - Shutdown stuff for the application...
	TearDownGameEngine();

	AppDeinit();
}

void MusaApp::ResizeWindow(const IntVector2& newDimensions)
{
	appWindow->Resize(newDimensions.x, newDimensions.y);
}

void MusaApp::CloseWindow()
{
	//gameEngine->StopRunningEngine();
	gIsRunning = false;
	appWindow.Reset();
}

void MusaApp::Activation(bool /*activated*/)
{
}

void MusaApp::LockCursor()
{
	IntVector2 position = appWindow->GetPosition();
	Recti rect = {};
	rect.x = position.x;
	rect.y = position.y;
	rect.width = appWindow->GetWidth();
	rect.height = appWindow->GetHeight();
	osApp->LockCursorToRect(rect);
}

void MusaApp::UnlockCursor()
{
	osApp->UnlockCursorFromRect();
}

void MusaApp::ShowCursor(bool showCursor)
{
	osApp->ShowCursor(showCursor);
}

void MusaApp::SetMousePosition(const IntVector2& mousePos)
{
	osApp->SetMousePosition(mousePos);
}

IntVector2 MusaApp::GetMousePosition() const
{
	return osApp->GetMousePosition();
}

void MusaApp::InitializeOSInput()
{
	inputMap = MakeUnique<ApplicationInputMap>(*this);
	inputDispatcher = MakeUnique<ApplicationEventDispatcher>(*this);
	Input::InitializeInput(*inputMap);

	osApp = new MusaAppWindows(*this);
}

void MusaApp::InitializeApplicationWindow()
{
	MUSA_INFO(AppLog, "Initializing App Window");
	Assert(osApp);
	appWindow = osApp->CreateGameWindow(0, 0, windowWidth, windowHeight);
	Assert(appWindow.IsValid());

	MUSA_DEBUG(AppLog, "Hooking up Input to Window");
	uiContext->SetWindow(*appWindow);
}

void MusaApp::SetupGameEngine()
{
	gameEngine = MakeUnique<MusaEngine>(*uiContext, *inputDispatcher);
	gameEngine->SetInputHandler(*inputMap);

	gameEngine->StartupEngine(*appWindow);

	gameEngine->LoadContent();

	// HACK - This sort of behavior shouldn't be exposed at this level!
	// TODO - This sort of was a quick hack just to test out application level events for switching application windows. 
	// This needs to be revisited eventually
	//osApp->GetInputHandler()->SetInputFocusToGame();

	frameTick.Start();
}

void MusaApp::TearDownGameEngine()
{
	gameEngine->UnloadContent();
	gameEngine->ShutdownEngine();
	gameEngine.Reset();
}

void MusaApp::ApplicationUpdate()
{
	frameTick.Lap();
	const f32 tick = (f32)frameTick.GetSeconds();
	frameTick.Start();

	Frame::SetFrameStats({ tick });

	// Process input
	ProcessApplicationInputs();

	gameEngine->UpdateAndRender(tick);

	gameEngine->GatherFrameMetrics();
}

void MusaApp::ProcessApplicationInputs()
{
	inputDispatcher->ClearCachedInputs();
	osApp->ProcessInputEvents();
	osApp->ProcessNativeGamepad();
}