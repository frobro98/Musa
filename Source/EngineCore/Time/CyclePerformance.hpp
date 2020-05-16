// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"

using Cycles = uint64;

Cycles GetCycleCount();
float64 GetMicrosecondsFrom(Cycles cycles);
float64 GetMillisecondsFrom(Cycles cycles);
float64 GetSecondsFrom(Cycles cycles);

