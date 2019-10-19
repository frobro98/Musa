#include "MusaApp.hpp"

MusaApp::MusaApp()
{
	uiContext = MakeUnique<GameUIContext>();
	gameEngine = MakeUnique<MusaEngine>(*uiContext);
}

void MusaApp::LaunchApplication()
{
	const int32 width = 1080;
	const int32 height = 720;

	appWindow = MakeUnique<Window>(width, height);
	appWindow->Initialize();
	appWindow->SetAsActiveWindow();

	gameEngine->InitializeGraphics();
	gameEngine->SetupWindowContext(*appWindow);

	gameEngine->RunEngine();
}
