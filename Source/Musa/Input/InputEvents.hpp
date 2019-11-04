#pragma once

#include <optional>

#include "Math/IntVector2.hpp"

enum HandledInput
{
	Handled
};

// Describes the input state as it is after some form of input is handled
// THESE EVENTS DESCRIBE APPLICATION LEVEL EVENTS THAT HAPPEN
// TODO - Consider renaming this to reflect that it's all about operating system level events that are important here
class InputEvents
{
public:
	InputEvents() = default;
	InputEvents(HandledInput);
	// Purpose for this is to communicate from UI and Game up to the input processing
	// Will be used for setting input captures\

	// Input captures primarily are for mouse but could be for other types of input. 
	// When an input event is handled and then something is told to be captured, it must let
	// the previous capture know. Because UI is processed from leaf to root, this allows the 
	// game input to potentially be locked out of some input. This will be done via capture system.

	bool IsInputHandled() const { return handled; }
	bool ContainsEvents() const { return containsEvents; }

	void ChangeMousePosition(IntVector2 pos);
	void LockCursor(bool lock);
	void ShowCursor(bool show);

	std::optional<IntVector2> GetChangedMousePosition() const { return changedMousePositoin; }
	std::optional<bool> GetLockCursor() const { return lockCursor; }
	std::optional<bool> GetShowCursor() const { return showCursor; }

	// No operator because there are multiple bools that this could represent...
/*	operator bool() const { return handled; }*/

private:
	std::optional<IntVector2> changedMousePositoin;
	std::optional<bool> lockCursor;
	std::optional<bool> showCursor;
	bool handled = false;
	bool containsEvents = false;
};

