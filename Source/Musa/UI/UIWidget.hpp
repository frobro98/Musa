#pragma once

#include "Containers/DynamicArray.hpp"
#include "Input/IInputReceiver.hpp"

// A widget is just a general term for whatever will exist in UI.
// It could be some sort of interactable widget like a button or it could be something that is only used for layout. 
struct UIWidget : public IInputReceiver
{
	// TODO - Not all widgets probably need children. Children needs to be fleshed out more
	// Widget's children
	DynamicArray<UIWidget*> children;

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

	// TODO - I'm not sure what the hierarchy in the input system. It makes sense to have input hierarchy in general, but it might be too close to the input system....
	virtual IInputReceiver* GetParentInput() const = 0;
	virtual IInputReceiver* GetChildInput() const = 0;

	// UI needs some sort of transform for rendering

	// UI needs position information relative to the parent
	// UI needs dimension information (width and height)
	// UI needs an anchor point and pivot point

	// UI must implement the IInputReceiver interface to receive all the different input
};

