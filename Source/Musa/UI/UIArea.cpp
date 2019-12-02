
#include "UIArea.hpp"

namespace UI
{
void Area::AddWidget(Widget& w)
{
	WidgetAndOrder wo = {
		&w,
		-1
	};
	children.Add(wo);
}
void Area::AddWidgetAt(Widget& w, uint32 zOrder)
{
	WidgetAndOrder wo = {
		&w,
		(int32)zOrder
	};
	
	uint32 i;
	for (i = 0; i < children.Size(); ++i)
	{
		WidgetAndOrder& child = children[i];
		if (child.zOrder > (int32)zOrder)
		{
			break;
		}
	}
	children.Insert(wo, i);
}
void Area::RemoveWidgetAt(int32 zOrder)
{
	for (uint32 i = 0; i < children.Size(); ++i)
	{
		WidgetAndOrder& child = children[i];
		if (child.zOrder == zOrder)
		{
			children.Remove(i);
			break;
		}
	}
}
}