#pragma once

#include "InputDefinitions.hpp"
#include "Input/InputEvents.hpp"

class IInputReceiver
{
public:
	virtual InputEvents OnControllerAnalogChange(uint32 controllerIndex, Inputs::Type analogInput, float32 analogValue) = 0;
	virtual InputEvents OnMouseMove(const IntVector2& currentMousePos, const IntVector2& prevMousePos, const IntVector2& delta) = 0;
	virtual InputEvents OnMouseDown(Inputs::Type input) = 0;
	virtual InputEvents OnMouseUp(Inputs::Type input) = 0;
	virtual InputEvents OnKeyDown(Inputs::Type input, bool isRepeated) = 0;
	virtual InputEvents OnKeyUp(Inputs::Type input) = 0;
	virtual InputEvents OnChar(tchar c, bool isRepeated) = 0;
	virtual InputEvents OnFocusReceived() = 0;
	virtual InputEvents OnFocusLost() = 0;

	virtual void OnActivationChanged(bool activated) = 0;

	// These functions aren't actually going to be needed because the input system will only contain the current UI path
};
