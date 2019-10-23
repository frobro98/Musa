#pragma once

#include "Engine/MusaAppOS.hpp"

class MusaAppWindows final : public MusaAppOS
{
public:
	MusaAppWindows();
	~MusaAppWindows();

	virtual Window* CreateGameWindow(uint32 xPos, uint32 yPos, uint32 width, uint32 height) override;

private:
	HINSTANCE instance;
};