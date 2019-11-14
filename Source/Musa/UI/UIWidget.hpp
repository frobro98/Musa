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

	// UI needs some sort of transform for rendering

	// UI needs position information relative to the parent
	// UI needs dimension information (width and height)
	// UI needs an anchor point and pivot point

	// UI must implement the IInputReceiver interface to receive all the different input
};

