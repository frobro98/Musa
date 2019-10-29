#pragma once

#include "InputDefinitions.hpp"

class IInputReceiver
{
public:
	virtual void OnMouseMove(uint32 mouseX, uint32 mouseY) = 0;
	virtual void OnMouseDown(Inputs::Type input) = 0;
	virtual void OnMouseUp(Inputs::Type input) = 0;
	virtual void OnKeyDown(Inputs::Type input, bool isRepeated) = 0;
	virtual void OnKeyUp(Inputs::Type input) = 0;
	virtual void OnChar(tchar c, bool isRepeated) = 0;
	virtual void OnFocusReceived() = 0;
	virtual void OnFocusLost() = 0;

	virtual void OnActivationChanged(bool activated) = 0;

	// TODO - I'm not sure what the hierarchy in the input system. It makes sense to have input hierarchy in general, but it might be too close to the input system....
	virtual IInputReceiver* GetParentInput() const = 0;
	virtual IInputReceiver* GetChildInput() const = 0;
	virtual IInputReceiver* GetSiblingInput() const = 0;
};
