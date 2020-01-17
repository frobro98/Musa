
#include "UIText.hpp"

namespace UI
{
Text::Text(FontID fontID)
	: font(GetLoadedFont(fontID))
{
}

void Text::UpdateInternal()
{
}

void Text::PrepareForRenderInternal()
{
}
}
