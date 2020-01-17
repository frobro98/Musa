#pragma once

// TODO - UI::Image is not a good description for what this is. It should be able to be an image/texture and/or a solid color value
// Both UE4 and Unity have an Image UI component, essentially boiling down to this behavior, but it seems weird to associate it with an image...

#include "UI/UIWidget.hpp"
#include "Color.hpp"

namespace UI
{
struct Image : public Widget
{
	virtual void UpdateInternal() override;
	virtual void PrepareForRenderInternal() override;

	Color color = Color::White();
	// Texture...???
};
}
