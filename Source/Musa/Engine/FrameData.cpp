
#include "FrameData.hpp"
#include "Internal/FrameDataInternal.hpp"
#include "MemoryUtilities.h"

namespace
{
static FrameData frameData;
}

namespace Frame
{
void SetFrameStats(const FrameData& frame)
{
	Memcpy(&frameData, sizeof(FrameData), &frame, sizeof(FrameData));
}

FrameData GetFrameData()
{
	return frameData;
}

float32 GetTickTimeSeconds()
{
	return frameData.tickSeconds;
}
}