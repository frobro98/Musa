#pragma once

#include "PlatformDefinitions.h"

namespace Math
{
	constexpr float Zero = 0.0f;						// ZERO 
	constexpr float One = 1.0f;							// ONE 

	constexpr float Pi = 3.1415926535f;					// PI 
	constexpr float PiOver2 = 1.5707963268f;			// PI / 2 
	constexpr float PiOver3 = 1.0471975512f;			// PI / 3 
	constexpr float PiOver4 = 0.7853981633f;			// PI / 4
	constexpr float PiOver8 = 0.3926990816f;			// PI / 8 

	// TODO - Find a better naming convention for these. Also determine if they're needed...
	constexpr float TwoPi = 6.2831853071f;				// 2 * PI 
	constexpr float TwoPiOver3 = 2.0943951023f;			// 2 * PI / 3 
	constexpr float TwoPiOver8 = 0.7853981633f;			// 2 * PI / 8 
	constexpr float ThreePiOver4 = 2.3561944901f;			// 3 * PI / 4 
	constexpr float ThreePiOver8 = 1.1780972451f;		// 3 * PI / 8 
	constexpr float FourPiOver8 = 1.5707963268f;		// 4 * PI / 8 
	constexpr float FivePiOver8 = 1.9634954085f;		// 5 * PI / 8 
	constexpr float SixPiOver8 = 2.3561944901f;			// 6 * PI / 8 
	constexpr float SevenPiOver8 = 2.7488935718f;		// 7 * PI / 8 
	
	constexpr float InverseSqrtOf2 = 0.707106781188f;	// constant for 1/sqrt(2) 

	// TODO - need to figure out if this should be defined or not
	constexpr float InternalTolerence = 0.0001f;        // constant for internal tolerances

	constexpr float RadiansToDegrees(float rad)
	{
		return rad * 57.2957795147f; // 180 / PI 
	}

	constexpr float DegreesToRadians(float deg)
	{
		return deg * 0.0174532925f; // Pi / 180 
	}
}
