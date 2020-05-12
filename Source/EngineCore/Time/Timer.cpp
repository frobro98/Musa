// Copyright 2020, Nathan Blane

#include "Timer.h"

void Timer::Tic()
{
	this->privTicMark = Timer::privGetSystemTime();
}


const Time Timer::Toc() const
{
	Time elapsedTime;

	// If tick has been called...
	if ( Time(Time::MAX) != this->privTicMark )
	{
		elapsedTime = Timer::privGetSystemTime() - this->privTicMark;
	}

	return( elapsedTime );
}

