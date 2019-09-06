
#include "CyclePerformance.hpp"

static float64 InvCyclesPerSec = 0.0;

Cycles GetCycleCount()
{
	LARGE_INTEGER cycles;
	QueryPerformanceCounter(&cycles);
	return (Cycles)cycles.QuadPart;
}

float64 GetMicrosecondsFrom(Cycles cycles)
{
	if (InvCyclesPerSec == 0)
	{
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);
		InvCyclesPerSec = 1. / freq.QuadPart;
	}

	cycles *= 1000000;
	float64 microsec = cycles * InvCyclesPerSec;
	return microsec;
}

float64 GetMillisecondsFrom(Cycles cycles)
{
	return GetMicrosecondsFrom(cycles) / 1000.;
}

float64 GetSecondsFrom(Cycles cycles)
{
	return GetMicrosecondsFrom(cycles) / 1000000.;
}
