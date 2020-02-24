
#include "UIImage.hpp"

namespace UI
{
Image::Image(Widget* parent)
	: Widget(parent)
{
}
void Image::UpdateInternal()
{
}
void Image::PrepareForRenderInternal(WidgetBatchElements& widgetElements)
{
	UNUSED(widgetElements);
}
}