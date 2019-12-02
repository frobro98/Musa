#pragma once

#include "Containers/DynamicArray.hpp"
#include "Input/IInputReceiver.hpp"
#include "Math/Vector2.hpp"
#include "Math/Matrix2.hpp"

namespace UI
{
// A widget is just a general term for whatever will exist in UI.
// It could be some sort of interactable widget like a button or it could be something that is only used for layout. 
class Widget : public IInputReceiver
{
public:
	Widget() = default;
	Widget(Widget* parentUI) : parent(parentUI) {}
	virtual ~Widget() = default;


	// UI must implement the IInputReceiver interface to receive all the different input
	virtual InputEvents OnControllerAnalogChange(uint32 /*controllerIndex*/, Inputs::Type /*analogInput*/, float32 /*analogValue*/) { return InputEvents{}; }
	virtual InputEvents OnMouseMove(const IntVector2& /*currentMousePos*/, const IntVector2& /*prevMousePos*/, const IntVector2& /*delta*/) { return InputEvents{}; }
	virtual InputEvents OnMouseEnter(const IntVector2& /*currentMousePos*/, const IntVector2& /*prevMousePos*/, const IntVector2& /*delta*/) { return InputEvents{}; }
	virtual InputEvents OnMouseExit(const IntVector2& /*currentMousePos*/, const IntVector2& /*prevMousePos*/, const IntVector2& /*delta*/) { return InputEvents{}; }
	virtual InputEvents OnMouseDown(Inputs::Type /*input*/) { return InputEvents{}; }
	virtual InputEvents OnMouseUp(Inputs::Type /*input*/) { return InputEvents{}; }
	virtual InputEvents OnKeyDown(Inputs::Type /*input*/, bool /*isRepeated*/) { return InputEvents{}; }
	virtual InputEvents OnKeyUp(Inputs::Type /*input*/) { return InputEvents{}; }
	virtual InputEvents OnChar(tchar /*c*/, bool /*isRepeated*/) { return InputEvents{}; }
	virtual InputEvents OnFocusReceived() { return InputEvents{}; }
	virtual InputEvents OnFocusLost() { return InputEvents{}; }

	virtual void OnActivationChanged(bool /*activated*/) {}

	inline void SetUIParent(Widget* parentUI) { parent = parentUI; }
	inline Widget* GetUIParent() const { return parent; }

private:

	Widget* parent = nullptr;

	// UI needs some sort of transform for rendering
	Matrix2 scaleRot;
	// UI needs position information relative to the parent
	Vector2 translation;

	// UI needs dimension information (width and height)
	Vector2 widgetDimensions;
	// UI needs an anchor point and pivot point

};
}
