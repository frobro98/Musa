// Copyright 2020, Nathan Blane

#include "Musa/Entry/MusaEntry.hpp"
#include "Musa/Application/MusaGameApp.hpp"

MusaApp* CreateMusaApplication(char* /*cmdlineArgs*/)
{
	return new MusaGameApp;
}


