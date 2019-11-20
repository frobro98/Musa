#pragma once

#include "Containers/DynamicArray.hpp"

struct UIWidget;

namespace UI
{

class Context
{
public:
	// Either needs to take in the input handler or whoever "adds" UI must also add it to the input handling
	Context() = default;

	// Adds widget tree to the context
	void AddWidget(UIWidget& widget);

	// Updates all of the UI within the tree. Probably pushes all of the info to the render side of things as well...
	void Update();

private:
	// Tree of widgets
	DynamicArray<UIWidget*> widgets;
};

}
