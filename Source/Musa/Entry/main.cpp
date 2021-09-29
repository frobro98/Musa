// Copyright 2020, Nathan Blane

#include "Application/Musa.hpp"
#include <shellapi.h>

extern MusaApp* CreateMusaApplication(char* cmdlineArgs);

MusaApp* gApp = nullptr;
bool gIsRunning = false;

int WINAPI WinMain(HINSTANCE /*hInstance*/,
	HINSTANCE /*hPrevInstance*/,
	LPSTR lpCmdLine,
	int /*nCmdShow*/)

{
	//int argc = 0;
	//char* argv = CommandLineToArgvW(lpCmdLine, &argc);
	gApp = CreateMusaApplication(lpCmdLine);
	gApp->LaunchApplication();

	return 0;
}
