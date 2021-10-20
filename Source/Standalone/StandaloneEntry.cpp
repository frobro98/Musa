// Copyright 2020, Nathan Blane

#include "Musa/Entry/MusaEntry.hpp"
#include "Game/Application/MusaGameApp.hpp"

MusaApp* CreateMusaApplication(char* /*cmdlineArgs*/)
{
	return new MusaGameApp;
}


