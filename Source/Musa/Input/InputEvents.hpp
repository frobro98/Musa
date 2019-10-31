#pragma once

// Describes the input state as it is after some form of input is handled
struct InputEvents
{
	bool containsEvents;
	// Purpose for this is to communicate from UI and Game up to the input processing
	// Will be used for setting input captures\

	// Input captures primarily are for mouse but could be for other types of input. 
	// When an input event is handled and then something is told to be captured, it must let
	// the previous capture know. Because UI is processed from leaf to root, this allows the 
	// game input to potentially be locked out of some input. This will be done via capture system.
};

