// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"

namespace Math
{
constexpr float32 Zero = 0.0f;							// ZERO 
constexpr float32 One = 1.0f;							// ONE 

constexpr float64 Infinity = 1e+300 * 1e+300;

constexpr float64 NatLogOf2_dbl = 0.693147180559945309;
constexpr float64 NatLogOf10_dbl = 2.302585092994045684;
constexpr float32 NatLogOf2 = (float32)NatLogOf2_dbl;
constexpr float32 NatLogOf10 = (float32)NatLogOf10_dbl;

constexpr float64 SqrtOf2_dbl = 1.414213562373095;
constexpr float64 InvSqrtOf2_dbl = 1.f / SqrtOf2_dbl;

// Pi
constexpr float64 Pi_dbl =		3.14159265358979323846;
constexpr float64 PiOver2_dbl = 1.57079632687948966192;
constexpr float64 PiOver3_dbl = 1.04719755119659774615;
constexpr float64 PiOver4_dbl = 0.78539816339744830961;
constexpr float64 PiOver8_dbl = 0.39269908169872415481;

constexpr float64 TwoPi_dbl = 6.28318530717958647692;
constexpr float64 TwoPiOver3_dbl = 2.09439510239319549231;
constexpr float64 ThreePiOver4_dbl = 2.35619449019234492885;
constexpr float64 ThreePiOver8_dbl = 1.17809724509617246442;
constexpr float64 FivePiOver8_dbl = 1.96349540849362077404;
constexpr float64 SevenPiOver8_dbl = 2.74889357189106908365;

constexpr float64 PiOver180_dbl = 0.01745329251994329577;
constexpr float64 InvPiOver180_dbl = 57.2957795130823208768;

constexpr float32 Pi = (float32)Pi_dbl;
constexpr float32 PiOver2 = (float32)PiOver2_dbl;
constexpr float32 PiOver3 = (float32)PiOver3_dbl;
constexpr float32 PiOver4 = (float32)PiOver4_dbl;
constexpr float32 PiOver8 = (float32)PiOver8_dbl;

constexpr float32 TwoPi = (float32)TwoPi_dbl;
constexpr float32 TwoPiOver3 = (float32)TwoPiOver3_dbl;
constexpr float32 ThreePiOver4 = (float32)ThreePiOver4_dbl;
constexpr float32 ThreePiOver8 = (float32)ThreePiOver8_dbl;
constexpr float32 FivePiOver8 = (float32)FivePiOver8_dbl;
constexpr float32 SevenPiOver8 = (float32)SevenPiOver8_dbl;

constexpr float32 PiOver180 = (float32)PiOver180_dbl;
constexpr float32 InvPiOver180 = (float32)InvPiOver180_dbl;

constexpr float32 SqrtOf2 = (float32)SqrtOf2_dbl;
constexpr float32 InvSqrtOf2 = (float32)InvSqrtOf2_dbl;

constexpr float32 InternalTolerence = 0.0001f;        // constant for internal tolerances

}
