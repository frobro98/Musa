#pragma once

#include "Containers/DynamicArray.hpp"
#include "UI/UIWidget.hpp"
#include "Math/Rect.hpp"

namespace UI
{
// Describes a specific region of the UI interface
class Area : public Widget
{
public:

private:
	virtual void UpdateInternal() override;
	virtual void PrepareRenderInternal() override;

private:


};
}

