
#include "InputEvents.hpp"

InputEvents::InputEvents(HandledInput)
	: handled(true)
{
}

void InputEvents::ChangeMousePosition(IntVector2 pos)
{
	containsEvents = true;
	changedMousePositoin = pos;
}

void InputEvents::LockCursor(bool lock)
{
	containsEvents = true;
	lockCursor = lock;
}
