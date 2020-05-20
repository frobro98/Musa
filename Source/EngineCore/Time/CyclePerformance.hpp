// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "CoreAPI.hpp"

using Cycles = uint64;

CORE_API Cycles GetCycleCount();
CORE_API float64 GetMicrosecondsFrom(Cycles cycles);
CORE_API float64 GetMillisecondsFrom(Cycles cycles);
CORE_API float64 GetSecondsFrom(Cycles cycles);

