
#include "UIContext.hpp"
#include "Input/Input.hpp"

namespace UI
{
Context::Context(uint32 screenDimensionX, uint32 screenDimensionY)
	: screenWidth(screenDimensionX),
	screenHeight(screenDimensionY)
{
}
void Context::AddWidget(Widget& widget)
{
	UNUSED(widget);
}

void Context::Update()
{
	if (Input::IsPressed(Inputs::Key_F1))
	{

	}
}

Path Context::GetUIPathAt(uint32 screenPosX, uint32 screenPosY) const
{
	UNUSED(screenPosY, screenPosX);
	return Path();
}
}