#pragma once

#include <optional>

#include "Containers/DynamicArray.hpp"
#include "Input/IInputReceiver.hpp"
#include "Math/Vector2.hpp"
#include "Math/Matrix2.hpp"

namespace UI
{

// TODO - Figure out a way to represent gpu side data!!

// A widget is just a general term for whatever will exist in UI.
// It could be some sort of interactable widget like a button or it could be something that is only used for layout. 
struct Widget : public IInputReceiver
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

	// Widget public interface
	
	// Updates widget hierarchy
	void Update(const Vector2& parentPosition, const Vector2& parentScale);
	// Prepares widget hierarchy for render;
	void PrepareRender();

public:
	DynamicArray<Widget*> children;

	Widget* parent = nullptr;

	// UI needs position information relative to the parent
	Vector2 relativePosition;
	// UI needs some sort of transform for rendering
	Vector2 relativeScale; // Also represents the dimensions...???

	// UI needs an anchor point and pivot point
	Vector2 pivot;

	// Actual transforms within view space
	Vector2 absolutePosition;
	Vector2 absoluteScale;

private:
	void UpdateAbsoluteTransform(const Vector2& parentPosition, const Vector2& parentScale);

private:
	virtual void UpdateInternal() = 0;
	virtual void PrepareRenderInternal() = 0;
};
}
