#pragma once

#include "Platform.h"
#include "SkeletonHeader.h"
#include "Animation/KeyFrame.h"
#include "Time/Time.h"

// TODO - Move this file out of the Archiver project

constexpr const uint32 AnimationNameLength = 64;

struct AnimationHeader
{
	AnimationHeader()
	{
		memset(animationName, 0, AnimationNameLength);
	}

	tchar animationName[AnimationNameLength];
	uint32 referenceSkeleton;
	uint32 keyFrameCount;
	uint32 bonesPerFrame;
	uint32 totalAnimationTime;
	Time::Duration frameRate;
};