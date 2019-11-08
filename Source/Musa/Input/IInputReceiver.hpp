#pragma once

#include "InputDefinitions.hpp"
#include "Input/InputEvents.hpp"

class IInputReceiver
{
public:
	virtual InputEvents OnMouseMove(const IntVector2& currentMousePos, const IntVector2& prevMousePos, const IntVector2& delta) = 0;
	virtual InputEvents OnMouseDown(Inputs::Type input) = 0;
	virtual InputEvents OnMouseUp(Inputs::Type input) = 0;
	virtual InputEvents OnKeyDown(Inputs::Type input, bool isRepeated) = 0;
	virtual InputEvents OnKeyUp(Inputs::Type input) = 0;
	virtual InputEvents OnChar(tchar c, bool isRepeated) = 0;
	virtual InputEvents OnFocusReceived() = 0;
	virtual InputEvents OnFocusLost() = 0;

	virtual void OnActivationChanged(bool activated) = 0;

	// TODO - I'm not sure what the hierarchy in the input system. It makes sense to have input hierarchy in general, but it might be too close to the input system....
	virtual IInputReceiver* GetParentInput() const = 0;
	virtual IInputReceiver* GetChildInput() const = 0;
	virtual IInputReceiver* GetSiblingInput() const = 0;
};
