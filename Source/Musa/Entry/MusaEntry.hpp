// Copyright 2020, Nathan Blane

#pragma once

#include "Application/Musa.hpp"

extern MusaApp* CreateMusaApplication(char* cmdlineArgs);

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
