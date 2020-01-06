#pragma once

#include "Containers/DynamicArray.hpp"
#include "UI/UIWidget.hpp"
#include "Math/Rect.hpp"

namespace UI
{
// Describes a specific region of the UI interface
class Area : public Widget
{


private:
	virtual void UpdateInternal() override;

	struct WidgetAndOrder
	{
		Widget* widget;
		int32 zOrder;
	};

private:
	// Dimensions
	
	// TODO - Should dimensions start at top left corner?
	//Rect rect;

	// TODO - Should origin be in center of screen (view space) ?
	Vector2 origin;
	float32 width;
	float32 height;

};
}

