/*
 * Timer.cpp
 *
 * Common timer implementation across all platforms.
 */

//---------------------------------------------------------------------------
// HEADER FILES:
//---------------------------------------------------------------------------
#include "Timer.h"


//---------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTORS / ASSIGNMENT:
//---------------------------------------------------------------------------
Timer::Timer(): 
	privTicMark( Time::MAX )
{ }


Timer::~Timer()	
{ }


//---------------------------------------------------------------------------
// TIMING METHODS:
//---------------------------------------------------------------------------
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

