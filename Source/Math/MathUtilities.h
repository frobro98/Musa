#pragma once

#include "MathConstants.h"

namespace Math
{
	constexpr void Clamp(float& val, float a, float b)
	{
		val = val < a ? a : (val > b ? b : val);
	}

	constexpr bool IsEqual(const float a, const float b, const float epsilon = InternalTolerence)
	{
		float tmp = a - b;
		return ((tmp >= -epsilon) && (tmp <= epsilon));
	}
	
	constexpr bool IsNotEqual(const float a, const float b, const float epsilon = InternalTolerence)
	{
		float tmp = a - b;
		return ((tmp < -epsilon) || (tmp > epsilon));
	}
	
	constexpr bool IsOne(const float a, const float epsilon = InternalTolerence)
	{
		float tmp = a - 1.0f;
		return ((tmp >= -epsilon) && (tmp <= epsilon));
	}
	
	constexpr bool IsNonZero(const float a, const float epsilon = InternalTolerence)
	{
		return ((a < -epsilon) || (a > epsilon));
	}
	
	constexpr bool IsZero(const float a, const float epsilon = InternalTolerence)
	{
		return ((a >= -epsilon) && (a <= epsilon));
	}

	inline float Abs(float value)
	{
		return fabs(value);
	}

	inline int32 Abs(int32 value)
	{
		return abs(value);
	}

	inline float Sin(float radAngle)
	{
		return sinf(radAngle);
	}

	inline float Asin(float radAngle)
	{
		return asinf(radAngle);
	}

	inline float Cos(float radAngle)
	{
		return cosf(radAngle);
	}

	inline float Acos(float radAngle)
	{
		return acosf(radAngle);
	}

	inline float Tan(float radAngle)
	{
		return tanf(radAngle);
	}

	inline float Atan(float radAngle)
	{
		return atanf(radAngle);
	}

	inline float Sinc(float x)
	{
		float ans = 1;
		if (!IsZero(x))
		{
			ans = Sin(Math::Pi * x) / (Math::Pi * x);
		}
		return ans;
	}
	
	inline float Sqrt(float num)
	{
		return sqrtf(num);
	}

	inline float Pow(float num, float pow)
	{
		return powf(num, pow);
	}

	inline float Floor(float num)
	{
		return floorf(num);
	}

	inline float Ceil(float num)
	{
		return ceilf(num);
	}

	inline float Log2(float x)
	{
		return log2f(x);
	}
}
