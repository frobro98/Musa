// Copyright 2020, Nathan Blane

#include "EngineTick.h"

float EngineTick::CountsPerSec = 0.f;

EngineTick::EngineTick()
{
// 	LARGE_INTEGER Freq;
// 	QueryPerformanceFrequency(&Freq);
	// Only divides once
/*	CountsPerSec = 1.f / Freq.QuadPart;*/
}


EngineTick::~EngineTick()
{
}

void EngineTick::Start()
{
/*	QueryPerformanceCounter(&StartTime);*/
}

void EngineTick::Lap()
{
// 	QueryPerformanceCounter(&TickTime);
// 	DeltaTime.QuadPart = TickTime.QuadPart - StartTime.QuadPart;
}

// float64 EngineTick::GetSeconds() const
// {
// 	float64 timeElapsed = static_cast<float64>(DeltaTime.QuadPart);
// 	timeElapsed *= CountsPerSec;
// 	return timeElapsed;
// }
// 
// float64 EngineTick::GetMilliseconds() const
// {
// 	float64 timeElapsed = static_cast<float64>(DeltaTime.QuadPart);
// 	timeElapsed *= CountsPerSec * 1000;
// 	return timeElapsed;
// }
// 
// float64 EngineTick::GetMicroseconds() const
// {
// 	float64 timeElapsed = static_cast<float64>(DeltaTime.QuadPart);
// 	timeElapsed *= CountsPerSec * 1000000;
// 	return timeElapsed;
// }
