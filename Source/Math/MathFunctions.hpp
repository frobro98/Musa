// Copyright 2020, Nathan Blane

#pragma once

#include <cmath>

#include "CoreFlags.hpp"
#include "Math/MathConstants.hpp"
#include "Math/BitManipulation.hpp"
#include "Math/MathDll.hpp"

namespace Math
{
template<typename T>
constexpr forceinline T Max(T lhs, T rhs)
{
	return lhs > rhs ? lhs : rhs;
}

template<typename T>
constexpr forceinline T Min(T lhs, T rhs)
{
	return lhs < rhs ? lhs : rhs;
}

template<typename T>
constexpr forceinline T Clamp(T val, T minVal, T maxVal)
{
	return Min(Max(val, minVal), maxVal);
}

// Saturate
constexpr forceinline float32 Saturate(float32 val)
{
	return Clamp(val, 0.f, 1.f);
}

constexpr forceinline float64 Saturate(float64 val)
{
	return Clamp(val, 0., 1.);
}

// Select
constexpr forceinline int32 Select(int32 iFalse, int32 iTrue, bool expr)
{
	return expr ? iTrue : iFalse;
}

constexpr forceinline int64 Select(int64 iFalse, int64 iTrue, bool expr)
{
	return expr ? iTrue : iFalse;
}

constexpr forceinline uint32 Select(uint32 uFalse, uint32 uTrue, bool expr)
{
	return expr ? uTrue : uFalse;
}

constexpr forceinline uint64 Select(uint64 fFalse, uint64 fTrue, bool expr)
{
	return expr ? fTrue : fFalse;
}

constexpr forceinline float32 Select(float32 fFalse, float32 fTrue, bool expr)
{
	return expr ? fTrue : fFalse;
}

constexpr forceinline float64 Select(float64 fFalse, float64 fTrue, bool expr)
{
	return expr ? fTrue : fFalse;
}

constexpr forceinline bool IsEqual(const float32 a, const float32 b, const float32 epsilon = InternalTolerence)
{
	float32 tmp = a - b;
	return ((tmp >= -epsilon) && (tmp <= epsilon));
}
	
constexpr forceinline bool IsNotEqual(const float32 a, const float32 b, const float32 epsilon = InternalTolerence)
{
	return !IsEqual(a, b, epsilon);
}
	
constexpr forceinline bool IsOne(const float32 a, const float32 epsilon = InternalTolerence)
{
	float32 tmp = a - 1.0f;
	return ((tmp >= -epsilon) && (tmp <= epsilon));
}
	
constexpr forceinline bool IsNonZero(const float32 a, const float32 epsilon = InternalTolerence)
{
	return ((a < -epsilon) || (a > epsilon));
}
	
constexpr forceinline bool IsZero(const float32 a, const float32 epsilon = InternalTolerence)
{
	return ((a >= -epsilon) && (a <= epsilon));
}

constexpr forceinline float32 RadiansToDegrees(float32 rad)
{
	return rad * InvPiOver180;
}

constexpr forceinline float64 RadiansToDegrees(float64 rad)
{
	return rad * InvPiOver180_dbl;
}

constexpr forceinline float32 DegreesToRadians(float32 deg)
{
	return deg * PiOver180;
}

constexpr forceinline float64 DegreesToRadians(float64 deg)
{
	return deg * PiOver180_dbl;
}

// Absolute Value
forceinline int32 Abs(int32 value)
{
	return Max(-value, value);
}

forceinline int64 Abs(int64 value)
{
	return Max(-value, value);
}

// TODO - Make Abs constexpr
forceinline float32 Abs(float32 value)
{
	constexpr uint32 floatMask = 0x7fffffff;
	return AsFloat32(AsUint32(value) & floatMask);
}

forceinline float64 Abs(float64 value)
{
	constexpr uint64 doubleMask = 0x7fffffffffffffff;
	return AsFloat64(AsUint64(value) & doubleMask);
}

forceinline float32 Floor(float32 val)
{
	return floorf(val);
}

forceinline float64 Floor(float64 val)
{
	return floor(val);
}

forceinline float32 Ceil(float32 val)
{
	return ceilf(val);
}

forceinline float64 Ceil(float64 val)
{
	return ceil(val);
}

forceinline float32 Round(float32 val)
{
	return roundf(val);
}

forceinline float64 Round(float64 val)
{
	return round(val);
}

forceinline float32 Truncate(float32 val)
{
	return truncf(val);
}

forceinline float64 Truncate(float64 val)
{
	return trunc(val);
}

forceinline float32 Frac(float32 val)
{
	return val - Floor(val);
}

forceinline float64 Frac(float64 val)
{
	return val - Floor(val);
}

forceinline float32 Modf(float32 val, float32& i)
{
	i = Truncate(val);
	return val - i;
}

forceinline float64 Modf(float64 val, float64& i)
{
	i = Truncate(val);
	return val - i;
}

MATH_API float32 Fmod(float32 x, float32 y);

template <typename LerpType>
forceinline LerpType Lerp(const LerpType& a, const LerpType& b, float32 t)
{
	return LerpType(a + t * (b - a));
}

template <typename StepType>
forceinline StepType Step(const StepType& a, const StepType& b)
{
	return Select(0.f, 1.f, a >= b);
}

template <typename StepType>
forceinline StepType SmoothStep(const StepType& a, const StepType& b, const StepType& t)
{
	StepType d = Saturate((t - a) / (b - a));
	return d * d * (3.f - (2.f * d));
}

forceinline float32 Sin(float32 radAngle)
{
	return sinf(radAngle);
}

forceinline float32 Asin(float32 radAngle)
{
	return asinf(radAngle);
}

forceinline float32 Cos(float32 radAngle)
{
	return cosf(radAngle);
}

forceinline float32 Acos(float32 radAngle)
{
	return acosf(radAngle);
}

forceinline float32 Tan(float32 radAngle)
{
	return tanf(radAngle);
}

forceinline float32 Atan(float32 radAngle)
{
	return atanf(radAngle);
}

forceinline float32 Atan2(float32 y, float32 x)
{
	return atan2f(y, x);
}

// Normalized sinc function
forceinline float32 Sinc(float32 x)
{
	float32 ans = 1;
	if (!IsZero(x))
	{
		ans = Sin(Math::Pi * x) / (Math::Pi * x);
	}
	return ans;
}
	
forceinline float32 Sqrt(float32 num)
{
	return sqrtf(num);
}

forceinline float64 Sqrt(float64 num)
{
	return sqrt(num);
}

forceinline float32 InvSqrt(float32 num)
{
	return 1.f / Sqrt(num);
}

forceinline float64 InvSqrt(float64 num)
{
	return 1.f / Sqrt(num);
}

forceinline float32 Pow(float32 num, float32 pow)
{
	return powf(num, pow);
}

forceinline float32 Log2(float32 x)
{
	return log2f(x);
}

forceinline float32 Log10(float32 x)
{
	return log10f(x);
}

forceinline float32 Ln(float32 x)
{
	return logf(x);
}

forceinline float32 LogBase(float32 base, float32 x)
{
	return logf(x) / logf(base);
}

forceinline float32 Exp(float32 x)
{
	return expf(x);
}

forceinline float32 Exp2(float32 x)
{
	return Exp(x * NatLogOf2);
}

forceinline float32 Exp10(float32 x)
{
	return Exp(x * NatLogOf10);
}

forceinline float64 Exp(float64 x)
{
	return exp(x);
}

forceinline float64 Exp2(float64 x)
{
	return Exp(x * NatLogOf2);
}

forceinline float64 Exp10(float64 x)
{
	return Exp(x * NatLogOf10);
}

}
