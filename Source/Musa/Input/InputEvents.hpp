#pragma once

#include <optional>

#include "Math/IntVector2.hpp"

struct InputHandled_t
{
};

// Describes the input state as it is after some form of input is handled
class InputEvents
{
public:
	InputEvents() = default;
	InputEvents(InputHandled_t);
	// Purpose for this is to communicate from UI and Game up to the input processing
	// Will be used for setting input captures\

	// Input captures primarily are for mouse but could be for other types of input. 
	// When an input event is handled and then something is told to be captured, it must let
	// the previous capture know. Because UI is processed from leaf to root, this allows the 
	// game input to potentially be locked out of some input. This will be done via capture system.

	bool InputHandled() const { return handled; }
	bool ContainsEvents() const { return containsEvents; }

	void ChangeMousePosition(IntVector2 pos);

	std::optional<IntVector2> GetChangedMousePosition() { return changedMousePositoin; }

private:
	std::optional<IntVector2> changedMousePositoin;
	bool handled = false;
	bool containsEvents = false;
};

