#pragma once

#include "Types/UniquePtr.hpp"
#include "Containers/FixedArray.hpp"
#include "UI/UIWidget.hpp"
#include "UI/UITextInput.hpp"
#include "UI/UIImage.hpp"
#include "String/String.h"

struct Font;

namespace UI
{
// This is a big boy widget now!
class Console : public Widget
{
public:
	Console();
	Console(Widget* parent);

	virtual InputEvents OnKeyDown(Inputs::Type keyType, bool isRepeated) override;
	virtual InputEvents OnChar(tchar c, bool isRepeated) override;

	virtual void UpdateInternal() override;
	virtual void PrepareForRenderInternal(WidgetBatchElements& widgetElements) override;

private:
	void InitializeFont();

private:
	static constexpr uint32 MaxCharsPerFrame = 4;
	String textOnScreen;
	uint32 cursorPos = 0;

	// Might not need something this heavy for a console...(?)
	//UniquePtr<TextInput> consoleInput;
	UniquePtr<Image> background;
	Font* consoleFont;
};
}
