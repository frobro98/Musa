#pragma once

#include "Time.h"
#include "BasicTypes/Uncopyable.hpp"
#include "CoreAPI.hpp"

class CORE_API Timer : private Uncopyable
{
public:
	/*
	* Marks the moment in time that toc() will get measured against.
	* Call tic() to mark the beginning of the segment of time that
	* you want to measure.
	*/
	void Tic();

	/*
	* return: The duration of Time that has elapsed since tic() was
	* called.  Returns Time::Time() zero if tic() has
	* never been called on this Timer.
	*/
	const Time Toc() const;


private:
	// static method
	static const Time privGetSystemTime();

	// Data
	Time privTicMark{ Time::MAX };
};
