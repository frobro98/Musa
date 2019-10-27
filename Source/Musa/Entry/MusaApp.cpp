#include "MusaApp.hpp"
#include "Engine/MusaAppWindows.hpp"

MusaApp::MusaApp()
{
	uiContext = MakeUnique<GameUIContext>();
	gameEngine = MakeUnique<MusaEngine>(*uiContext);
	inputHandler = MakeUnique<WindowInputHandler>(*this);
	osApp = new MusaAppWindows();
	// TODO - figure out if which level the input handler should live on...
	osApp->SetInputHandler(inputHandler.Get());
}

void MusaApp::LaunchApplication()
{
	const int32 width = 1080;
	const int32 height = 720;

	appWindow = osApp->CreateGameWindow(0, 0, width, height);
	inputHandler->SetCurrentWindow(*appWindow);

	gameEngine->InitializeGraphics();
	gameEngine->SetupWindowContext(*appWindow);

	gameEngine->RunEngine();
}
