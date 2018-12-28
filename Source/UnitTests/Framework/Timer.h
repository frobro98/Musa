//----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------
// Timer v.2.1.0
//---------------------------------------------------------------------------- 

#ifndef TIMER_H
#define TIMER_H

#include "EngineCore/Platform.h"

class Timer
{
public:
	// big four
	Timer();
	Timer(const Timer &) = delete;
	Timer & operator= (const Timer &) = delete;
	~Timer() = default;

	// methods
	void Tic();
	void Toc();
	void Reset();
	float TimeInSeconds();

private:	
	// methods
	LARGE_INTEGER	privGetTimer();
	void			privInitTimer();

	// data
	LARGE_INTEGER	ticTime;
	LARGE_INTEGER	tocTime;
	LARGE_INTEGER	deltaTime;
	float			SecondsPerCycle;
	float			timeSeconds;

};

#endif TIMER_H

// ---  End of File ---------------
