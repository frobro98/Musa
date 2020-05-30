// Copyright 2020, Nathan Blane

#pragma once

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "Platform/Platform.hpp"
#include "CoreAPI.hpp"

// TODO: Decouple OS specific methods from interface

class CORE_API EngineTick
{
public:
	EngineTick();
	~EngineTick();

	void Start();
	void Lap();

	f64 GetSeconds() const;
	f64 GetMilliseconds() const;
	f64 GetMicroseconds() const;

private:
	static float CountsPerSec;

	LARGE_INTEGER StartTime{};
	LARGE_INTEGER TickTime{};
	LARGE_INTEGER DeltaTime{};
};