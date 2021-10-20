
#include "Entry/MusaEntry.hpp"
#include "MusaEditorApp.hpp"

MusaApp* CreateMusaApplication(char* /*cmdlineArgs*/)
{
	return new MusaEditorApp;
}

void MusaEditorApp::AppInit()
{
}

void MusaEditorApp::AppLoop(f32 /*tick*/)
{
}

void MusaEditorApp::AppDeinit()
{
}
