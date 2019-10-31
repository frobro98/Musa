
#include "InputEvents.hpp"

InputEvents::InputEvents(InputHandled_t)
	: handled(true)
{
}

void InputEvents::ChangeMousePosition(IntVector2 pos)
{
	containsEvents = true;
	changedMousePositoin = pos;
}
