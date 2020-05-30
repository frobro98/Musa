// Copyright 2020, Nathan Blane

#include "EngineTick.h"

float EngineTick::CountsPerSec = 0.f;

EngineTick::EngineTick()
{
	LARGE_INTEGER Freq;
	QueryPerformanceFrequency(&Freq);
	// Only divides once
	CountsPerSec = 1.f / Freq.QuadPart;
}


EngineTick::~EngineTick()
{
}

void EngineTick::Start()
{
	QueryPerformanceCounter(&StartTime);
}

void EngineTick::Lap()
{
	QueryPerformanceCounter(&TickTime);
	DeltaTime.QuadPart = TickTime.QuadPart - StartTime.QuadPart;
}

f64 EngineTick::GetSeconds() const
{
	f64 timeElapsed = static_cast<f64>(DeltaTime.QuadPart);
	timeElapsed *= CountsPerSec;
	return timeElapsed;
}

f64 EngineTick::GetMilliseconds() const
{
	f64 timeElapsed = static_cast<f64>(DeltaTime.QuadPart);
	timeElapsed *= CountsPerSec * 1000;
	return timeElapsed;
}

f64 EngineTick::GetMicroseconds() const
{
	f64 timeElapsed = static_cast<f64>(DeltaTime.QuadPart);
	timeElapsed *= CountsPerSec * 1000000;
	return timeElapsed;
}
