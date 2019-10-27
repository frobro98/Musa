#pragma once

#include "InputDefinitions.hpp"

class IInputReceiver
{
public:
	virtual void OnMouseMove(uint32 mouseX, uint32 mouseY) = 0;
	virtual void OnKeyDown(Inputs::Type input, bool isRepeated) = 0;
	virtual void OnKeyUp(Inputs::Type input) = 0;
	virtual void OnChar(tchar c, bool isRepeated) = 0;
};
