#pragma once


namespace UI
{
struct Widget;

void AddWidgetToView(Widget& widget);
void RemoveWidgetFromView(Widget& widget);

template<class WidgetType, typename ...Args>
WidgetType* MakeWidget(Args... args)
{
	return new WidgetType(args);
}

}