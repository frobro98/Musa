// Copyright 2020, Nathan Blane

#include "CyclePerformance.hpp"
#include "Platform/Windows/WindowsDefinitions.h"

static f64 InvCyclesPerSec = 0.0;

Cycles GetCycleCount()
{
	LARGE_INTEGER cycles;
	QueryPerformanceCounter(&cycles);
	return (Cycles)cycles.QuadPart;
}

f64 GetMicrosecondsFrom(Cycles cycles)
{
	if (InvCyclesPerSec == 0)
	{
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);
		InvCyclesPerSec = 1. / freq.QuadPart;
	}

	cycles *= 1000000;
	f64 microsec = cycles * InvCyclesPerSec;
	return microsec;
}

f64 GetMillisecondsFrom(Cycles cycles)
{
	return GetMicrosecondsFrom(cycles) / 1000.;
}

f64 GetSecondsFrom(Cycles cycles)
{
	return GetMicrosecondsFrom(cycles) / 1000000.;
}
