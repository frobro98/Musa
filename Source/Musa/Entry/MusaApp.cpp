#include "MusaApp.hpp"
#include "Engine/MusaAppWindows.hpp"
#include "Engine/FrameData.hpp"
#include "Engine/Internal/FrameDataInternal.hpp"
#include "Input/Internal/InputInternal.hpp"
#include "Shader/ShaderDefinition.hpp"
#include "Graphics/GraphicsInterface.hpp"

constexpr int32 width = 1080;
constexpr int32 height = 720;

MusaApp::MusaApp()
{
	uiContext = MakeUnique<UI::Context>(width, height);
	gameEngine = MakeUnique<MusaEngine>(*uiContext);
}

void MusaApp::LaunchApplication()
{
	InitializeOSInput();
	InitializeApplicationWindow();

	GetGraphicsInterface().InitializeGraphics();
	InitializeShaders();

	SetupGameEngine();

	// TODO - Should have an engine level boolean, so as to not be dependent on the window's state
	while (gameEngine->ShouldRun())
	{
		ApplicationUpdate();
	}

	// TODO - Shutdown stuff for the application...
	TearDownGameEngine();

	GetGraphicsInterface().DeinitializeGraphics();
	
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

IntVector2 MusaApp::GetMousePosition() const
{
	return osApp->GetMousePosition();
}

void MusaApp::InitializeOSInput()
{
	Input::InitializeInput(*this);

	auto inputHandler = MakeUnique<WindowInputHandler>(*this, gameEngine->GetGameInput());
	osApp = new MusaAppWindows(std::move(inputHandler));
}

void MusaApp::InitializeApplicationWindow()
{
	Assert(osApp);
	appWindow = osApp->CreateGameWindow(0, 0, width, height);
	Assert(appWindow.IsValid());

	osApp->GetInputHandler()->SetCurrentWindow(*appWindow);
	uiContext->SetWindow(*appWindow);
}

void MusaApp::SetupGameEngine()
{
	gameEngine->StartupEngine(*appWindow);

	gameEngine->LoadContent();

	osApp->GetInputHandler()->SetInputFocusToGame();

	gameEngine->StartRunningEngine();

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
	const float32 tick = (float32)frameTick.GetSeconds();
	frameTick.Start();

	Frame::SetFrameStats({ tick });

	// Process input
	Internal::UpdateInputMap();
	osApp->ProcessInputEvents();
	osApp->ProcessNativeGamepad();
	osApp->PostProcessInputEvents();

	gameEngine->UpdateAndRender(tick);

	gameEngine->GatherFrameMetrics();
}
