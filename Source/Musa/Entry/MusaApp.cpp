#include "MusaApp.hpp"
#include "Engine/MusaAppWindows.hpp"
#include "Engine/FrameData.hpp"
#include "Engine/Internal/FrameDataInternal.hpp"

MusaApp::MusaApp()
{
	uiContext = MakeUnique<GameUIContext>();
	gameEngine = MakeUnique<MusaEngine>(*uiContext);
}

void MusaApp::LaunchApplication()
{
	InitializeOSInput();
	InitializeApplicationWindow();

	SetupGameEngine();

	// TODO - Should have an engine level boolean, so as to not be dependent on the window's state
	while (gameEngine->ShouldRun())
	{
		ApplicationUpdate();
	}

	// TODO - Shutdown stuff for the application...
}

void MusaApp::LockCursor()
{
	IntVector2 position = appWindow->GetPosition();
	IntRect rect = {};
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

void MusaApp::InitializeOSInput()
{
	InitializeInput();

	auto inputHandler = MakeUnique<WindowInputHandler>(*this, gameEngine->GetGameInput());
	osApp = new MusaAppWindows(std::move(inputHandler));
}

void MusaApp::InitializeApplicationWindow()
{
	const int32 width = 1080;
	const int32 height = 720;

	Assert(osApp);
	appWindow = osApp->CreateGameWindow(0, 0, width, height);
	osApp->GetInputHandler()->SetCurrentWindow(*appWindow);
}

void MusaApp::SetupGameEngine()
{
	gameEngine->InitializeGraphics();
	gameEngine->SetupWindowContext(*appWindow);
	gameEngine->InitializeSceneView();

	gameEngine->LoadContent();

	gameEngine->StartEngine();

	frameTick.Start();
}

void MusaApp::ApplicationUpdate()
{
	frameTick.Lap();
	const float32 tick = (float32)frameTick.GetSeconds();
	frameTick.Start();

	// Process input
	osApp->ProcessInputEvents();

	Frame::SetFrameStats({ tick });

	gameEngine->UpdateAndRenderWorld(tick);

	gameEngine->GatherFrameMetrics();
}
