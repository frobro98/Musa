#pragma once

#include "Containers/DynamicArray.hpp"
#include "UI/UIWidget.hpp"

namespace UI
{
// Describes a specific region of the UI interface
class Area : public Widget
{
public:
	
	// Adds widget at lowest z order value
	void AddWidget(Widget& w);
	void AddWidgetAt(Widget& w, uint32 zOrder = 0);
	void RemoveWidgetAt(int32 zOrder);

private:
	struct WidgetAndOrder
	{
		Widget* widget;
		int32 zOrder;
	};

private:
	// Dimensions

	// Children
	DynamicArray<WidgetAndOrder> children;

};
}

