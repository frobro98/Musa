#pragma once

#include "Containers/DynamicArray.hpp"
#include "UI/UIPath.hpp"

namespace UI
{
struct Widget;

class Context
{
public:
	// Either needs to take in the input handler or whoever "adds" UI must also add it to the input handling
	Context() = default;

	// Adds widget tree to the context
	void AddWidget(Widget& widget);

	// Updates all of the UI within the tree. Probably pushes all of the info to the render side of things as well...
	void Update();

	// Returns a ui tree path to the widget under
	Path GetUIPathAt(uint32 screenPosX, uint32 screenPosY) const;

private:
	// Tree of widgets
	DynamicArray<Widget*> widgets;
};

}
