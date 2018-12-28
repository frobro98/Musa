#include "EngineTick.h"

float EngineTick::CountsPerSec = 0.f;

EngineTick::EngineTick()
{

}


EngineTick::~EngineTick()
{
}

void EngineTick::Init()
{
	LARGE_INTEGER Freq;
	QueryPerformanceFrequency(&Freq);
	// Only divides once
	CountsPerSec = 1.f / Freq.QuadPart;
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

float EngineTick::GetSeconds() const
{
	float timeElapsed;
	timeElapsed = static_cast<float>(DeltaTime.QuadPart);
	timeElapsed *= CountsPerSec;
	return timeElapsed;
}

float EngineTick::GetMilliseconds() const
{
	float timeElapsed;
	timeElapsed = static_cast<float>(DeltaTime.QuadPart);
	timeElapsed *= CountsPerSec * 1000;
	return timeElapsed;
}

float EngineTick::GetMicroseconds() const
{
	float timeElapsed;
	timeElapsed = static_cast<float>(DeltaTime.QuadPart);
	timeElapsed *= CountsPerSec * 1000000;
	return timeElapsed;
}
