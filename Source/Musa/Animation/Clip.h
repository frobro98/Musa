#pragma once


#include "Time/Time.h"
#include "KeyFrame.h"

class AnimationMixer;

class AnimationClip
{
public:
	AnimationClip(const tchar* animationName, KeyFrame* frames, uint32 frameCount, Time animationTime, uint32 skelHash, Time::Duration frameRate);
	~AnimationClip();

	uint32 GetSkeletonHash() const;
	Time GetTotalAnimationTime() const;
	Time::Duration GetFrameRate() const;
	const char* GetName() const;

	const KeyFrame& GetKeyframe0() { return *keyframeA; }
	const KeyFrame& GetKeyframe1() { return *keyframeB; }

	void UpdateKeyframeRange(Time currentAnimTime);
	KeyFrame* InterpolateKeyframes(Time deltaTime);

private:
	static constexpr const size_t ClipNameLength = 64;
	char clipName[ClipNameLength];

	AnimationMixer* mixer;

	const KeyFrame* keyframeA;
	const KeyFrame* keyframeB;
	uint32 currentFrameBIndex;

	Time totalAnimationTime;
	const KeyFrame* animationFrames;
	uint32 skeletonHash;
	uint32 frameCount;

	Time::Duration frameRate;
};