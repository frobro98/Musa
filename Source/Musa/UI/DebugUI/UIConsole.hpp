#pragma once

#include "Types/UniquePtr.hpp"
#include "Containers/FixedArray.hpp"
#include "UI/UIWidget.hpp"
#include "UI/UITextInput.hpp"
#include "UI/UIImage.hpp"
#include "String/String.h"

namespace UI
{
// This is a big boy widget now!
class Console : public Widget
{
public:
	Console() = default;
	Console(Widget* parent);

	virtual InputEvents OnChar(tchar c, bool isRepeated) override;

private:
	virtual void UpdateInternal() override;
	virtual void PrepareForRenderInternal() override;

private:
	static constexpr uint32 MaxCharsPerFrame = 4;
	String textOnScreen;
	UniquePtr<TextInput> consoleInput;
	UniquePtr<Image> background;
};
}
