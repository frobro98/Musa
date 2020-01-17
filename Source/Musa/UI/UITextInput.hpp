#pragma once

#include "UI/UIWidget.hpp"
#include "String/String.h"

namespace UI
{
struct TextInput : public Widget
{
	virtual InputEvents OnChar(tchar character, bool isRepeated) override;

	String inputText;
	// Draw location for text as well as where the text will be next inserted
	uint32 cursorLocation;

private:
	virtual void UpdateInternal() override;
	virtual void PrepareForRenderInternal() override;
};
}

