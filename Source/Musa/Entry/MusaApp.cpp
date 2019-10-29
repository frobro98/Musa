#include "MusaApp.hpp"
#include "Engine/MusaAppWindows.hpp"

MusaApp::MusaApp()
{
	uiContext = MakeUnique<GameUIContext>();
	gameEngine = MakeUnique<MusaEngine>(*uiContext);
	auto inputHandler = MakeUnique<WindowInputHandler>(*this, gameEngine->GetGameInput());
	osApp = new MusaAppWindows(std::move(inputHandler));
}

void MusaApp::LaunchApplication()
{
	const int32 width = 1080;
	const int32 height = 720;

	appWindow = osApp->CreateGameWindow(0, 0, width, height);
	osApp->GetInputHandler()->SetCurrentWindow(*appWindow);

	gameEngine->InitializeGraphics();
	gameEngine->SetupWindowContext(*appWindow);

	gameEngine->RunEngine();
}
