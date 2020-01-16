#pragma once

#include "UI/UIWidget.hpp"

namespace UI
{
struct Button : public Widget
{
public:
	// Button mouse events
	virtual InputEvents OnMouseDown(Inputs::Type input) override;
	virtual InputEvents OnMouseUp(Inputs::Type input) override;
	virtual InputEvents OnMouseEnter(const IntVector2& currentMousePos, const IntVector2& prevMousePos, const IntVector2& delta) override;
	virtual InputEvents OnMouseExit(const IntVector2& currentMousePos, const IntVector2& prevMousePos, const IntVector2& delta) override;

public:
	// Button function events
	using ButtonEvent = void(*)(const Button&);

	ButtonEvent buttonClick = nullptr;
	ButtonEvent buttonHover = nullptr;
};
}
