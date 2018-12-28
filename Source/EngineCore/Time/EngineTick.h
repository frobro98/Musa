#pragma once

#pragma once

#include "Platform.h"

// TODO: Decouple OS specific methods from interface

class EngineTick
{
public:
	EngineTick();
	~EngineTick();

	// Called before all timers are used
	static void Init();

	void Start();
	void Lap();

	float GetSeconds() const;
	float GetMilliseconds() const;
	float GetMicroseconds() const;

private:
	static float CountsPerSec;

	LARGE_INTEGER StartTime;
	LARGE_INTEGER TickTime;
	LARGE_INTEGER DeltaTime;
};