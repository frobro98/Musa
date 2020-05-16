// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"

float32 NormalizeStickValue(int16 stickVal)
{
	const float32 denomVal = stickVal >= 0 ? 32767.f : 32768.f;
	return stickVal / denomVal;
}

float32 NormalizeTriggerValue(uint8 triggerValue)
{
	return triggerValue / 255.f;
}

