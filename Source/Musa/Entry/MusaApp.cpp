#include "MusaApp.hpp"
#include "Engine/MusaAppWindows.hpp"

MusaApp::MusaApp()
{
	uiContext = MakeUnique<GameUIContext>();
	gameEngine = MakeUnique<MusaEngine>(*uiContext);
	osApp = new MusaAppWindows();
}

void MusaApp::LaunchApplication()
{
	const int32 width = 1080;
	const int32 height = 720;

	appWindow = osApp->CreateGameWindow(0, 0, width, height);

	gameEngine->InitializeGraphics();
	gameEngine->SetupWindowContext(*appWindow);

	gameEngine->RunEngine();
}
