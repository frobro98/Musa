// Copyright 2020, Nathan Blane

#pragma once

#include "Types/Intrinsics.hpp"

struct FrameTime
{
	// tick for this frame (not including physics at this point...)
	const float32 tickSeconds = 0;
	// Maybe what global time the frame started at...???
};

namespace Frame
{
FrameTime GetFrameTime();
float32 GetTickTimeSeconds();
}