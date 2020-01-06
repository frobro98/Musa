#pragma once

#include "Containers/DynamicArray.hpp"
#include "UI/UIPath.hpp"

class Window;

namespace UI
{
struct Widget;

class Context
{
public:
	// Either needs to take in the input handler or whoever "adds" UI must also add it to the input handling
	Context(uint32 screenDimensionX, uint32 screenDimensionY);

	inline void SetNewDimensions(uint32 screenX, uint32 screenY)
	{
		screenWidth = screenX;
		screenHeight = screenY;
	}

	inline void SetWindow(Window& win) { window = &win; }
	inline Window* GetWindow() const { return window; }

	// Adds widget tree to the context
	void AddWidget(Widget& widget);

	// Updates all of the UI within the tree. Probably pushes all of the info to the render side of things as well...
	void Update();

	void PrepareUIForRender();

	// Returns a ui tree path to the widget under
	Path GetUIPathAt(uint32 screenPosX, uint32 screenPosY) const;

private:
	// Tree of widgets
	DynamicArray<Widget*> widgets;
	Window* window = nullptr;
	uint32 screenWidth;
	uint32 screenHeight;
};

}
