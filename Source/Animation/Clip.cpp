
#include "Clip.h"
#include "KeyFrame.h"
#include "AnimationMixer.h"
#include "String/CStringUtilities.hpp"
#include "MemoryUtilities.h"

AnimationClip::AnimationClip(const tchar* animationName, KeyFrame* frames, uint32 count, Time animationTime, uint32 skelHash, Time::Duration frameRate_)
	: mixer(new AnimationMixer(frames[0].boneCount)),
	totalAnimationTime(animationTime),
	animationFrames(frames),
	skeletonHash(skelHash),
	frameCount(count),
	frameRate(frameRate_)
{
	uint32 len = Strlen(animationName);
	if (len >= ClipNameLength)
	{
		len = ClipNameLength - 1;
	}
	Memcpy(clipName, ClipNameLength, animationName, len);
	clipName[len] = 0;

	keyframeA = &frames[0];
	keyframeB = &frames[1];
	currentFrameBIndex = 1;
}

AnimationClip::~AnimationClip()
{
}

uint32 AnimationClip::GetSkeletonHash() const
{
	return skeletonHash;
}

Time AnimationClip::GetTotalAnimationTime() const
{
	return totalAnimationTime;
}

Time::Duration AnimationClip::GetFrameRate() const
{
	return frameRate;
}

const char * AnimationClip::GetName() const
{
	return clipName;
}

KeyFrame* AnimationClip::InterpolateKeyframes(Time deltaTime)
{
	//UpdateKeyframeRange(deltaTime);

	mixer->InterpolateAnimationFrames(deltaTime, keyframeA, keyframeB);

	return mixer->GetResultingFrame();
}

void AnimationClip::UpdateKeyframeRange(Time currentAnimTime)
{
	if (currentAnimTime > keyframeB->frameTime)
	{
		if (currentAnimTime == totalAnimationTime)
		{
			currentFrameBIndex = frameCount - 1;
		}
		else
		{
			++currentFrameBIndex;
		}

		keyframeB = &animationFrames[currentFrameBIndex];
		keyframeA = &animationFrames[currentFrameBIndex - 1];
	}
	else if (currentAnimTime < keyframeA->frameTime)
	{
		if (currentAnimTime == Time(Time::ZERO))
		{
			currentFrameBIndex = 1;
		}
		else
		{
			--currentFrameBIndex;
		}

		keyframeB = &animationFrames[currentFrameBIndex];
		keyframeA = &animationFrames[currentFrameBIndex - 1];
	}
}
