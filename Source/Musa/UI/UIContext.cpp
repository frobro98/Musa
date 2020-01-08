
#include "UIContext.hpp"
#include "Input/Input.hpp"
#include "UIWidget.hpp"

namespace UI
{
Context::Context(uint32 screenDimensionX, uint32 screenDimensionY)
	: screenWidth(screenDimensionX),
	screenHeight(screenDimensionY)
{
}
void Context::AddWidget(Widget& widget)
{
	widgets.AddUnique(&widget);
}

void Context::RemoveWidget(Widget & widget)
{
	for (uint32 i = 0; i < widgets.Size(); ++i)
	{
		if (widgets[i] == &widget)
		{
			widgets.Remove(i);
			break;
		}
	}
}

void Context::Update()
{
	for (auto& element : widgets)
	{
		element->Update(Vector2::Zero, Vector2::One);
	}
}

void Context::PrepareUIForRender()
{
	for (auto& element : widgets)
	{
		element->PrepareRender();
	}
}

Path Context::GetUIPathAt(uint32 screenPosX, uint32 screenPosY) const
{
	UNUSED(screenPosY, screenPosX);
	return Path();
}
}