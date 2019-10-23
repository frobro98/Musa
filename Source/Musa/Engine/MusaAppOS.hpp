#pragma once

#include "Types/Intrinsics.hpp"

class Window;

class MusaAppOS
{
public:
	virtual ~MusaAppOS() = default;

	virtual Window* CreateGameWindow(uint32 xPos, uint32 yPos, uint32 width, uint32 height) = 0;

};