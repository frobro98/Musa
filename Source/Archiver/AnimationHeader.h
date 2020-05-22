// Copyright 2020, Nathan Blane

#pragma once

#include "SkeletonHeader.h"
#include "Animation/KeyFrame.h"
#include "Time/Time.h"

// TODO - Move this file out of the Archiver project

constexpr uint32 AnimationNameLength = 64;

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

forceinline void Deserialize(DeserializeBase& ser, AnimationHeader& header)
{
	ser.DeserializeData(header.animationName, AnimationNameLength);
	Deserialize(ser, header.referenceSkeleton);
	Deserialize(ser, header.keyFrameCount);
	Deserialize(ser, header.bonesPerFrame);
	Deserialize(ser, header.totalAnimationTime);
	uint32 duration;
	Deserialize(ser, duration);
	header.frameRate = (Time::Duration)duration;
}