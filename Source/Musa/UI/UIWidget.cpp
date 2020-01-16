#include "UIWidget.hpp"

namespace UI
{
void Widget::Update(const Vector2& parentPosition, const Vector2& parentScale)
{
	UpdateInternal();

	UpdateAbsoluteTransform(parentPosition, parentScale);

	for (auto& child : children)
	{
		child->Update(absolutePosition, absoluteScale);
	}
}

void Widget::PrepareRender()
{
	// PrepareRender prepares information used in the actual rendering of the UI.
	// If data changed from Update or other functions, that data needs to be pushed to relavent rendering structures.
	// 

	PrepareRenderInternal();

	// TODO - Decide on whether to have a bool that dictates whether the element should be rendered again
	for (auto& child : children)
	{
		child->PrepareRender();
	}
}

void Widget::UpdateAbsoluteTransform(const Vector2& parentPosition, const Vector2& parentScale)
{
	absolutePosition = relativePosition + parentPosition;
	absoluteScale = relativeScale * parentScale;
}
}