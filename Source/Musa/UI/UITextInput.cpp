
#include "UITextInput.hpp"
#include "UIText.hpp"

namespace UI
{
InputEvents TextInput::OnChar(tchar /*character*/, bool /*isRepeated*/)
{
	return InputEvents();
}
void TextInput::UpdateInternal()
{
}
void TextInput::PrepareForRenderInternal()
{
}
}


