#pragma once

#include "Platform.h"
#include "Time/Time.h"

struct KeyFrame;

class AnimationMixer
{
public:
	AnimationMixer(uint32 skelBoneCount);
	~AnimationMixer();

	void InterpolateAnimationFrames(Time deltaTime, const KeyFrame* frameA, const KeyFrame* frameB);
	KeyFrame* GetResultingFrame() const;

private:
	KeyFrame* resultingFrame;

};