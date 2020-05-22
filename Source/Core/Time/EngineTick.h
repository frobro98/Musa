// Copyright 2020, Nathan Blane

#pragma once

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "CoreAPI.hpp"

// TODO: Decouple OS specific methods from interface

class CORE_API EngineTick
{
public:
	EngineTick();
	~EngineTick();

	void Start();
	void Lap();

// 	float64 GetSeconds() const;
// 	float64 GetMilliseconds() const;
// 	float64 GetMicroseconds() const;

private:
	static float CountsPerSec;

// 	LARGE_INTEGER StartTime{};
// 	LARGE_INTEGER TickTime{};
// 	LARGE_INTEGER DeltaTime{};
};