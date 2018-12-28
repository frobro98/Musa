#pragma once

#include "Platform.h"
#include "Containers/Array.h"
#include "Time/Time.h"
#include "Clip.h"

class AnimationClip;
class AnimationTimeController;
class BlendSpace;
struct KeyFrame;

struct KeyFrameRange
{
	KeyFrameRange() = default;
	KeyFrameRange(const KeyFrame& f0, const KeyFrame& f1, float delta)
		: frame0(&f0), frame1(&f1), deltaTime(delta)
	{}
	const KeyFrame* frame0 = nullptr;
	const KeyFrame* frame1 = nullptr;
	float deltaTime = 0.f;
};

class AnimationController
{
public:
	AnimationController(Time::Duration frameRate);
	~AnimationController();

	void AddClip(AnimationClip* clip);
	void SetCurrentClip(const char* clipName);
	void SetCurrentClip(AnimationClip* clip);
	void SetCurrentClip(uint32 clipIndex);

	AnimationTimeController* CreateTimeController(uint32 clipIndex);
	void DestroyTimeController(AnimationTimeController* timer);
	void SetCurrentTimer(AnimationTimeController& timer);

	BlendSpace* StartBlending(uint32 currentIndex, uint32 newClipIndex);
	void DestroyBlendSpace(BlendSpace* blendSpace);

	void PlayAnimation();
	void PauseAnimation();

	KeyFrameRange GetFrameRange() const { return keyframeRange; }
	uint32 GetClipCount() const { return clips.Size(); }
	
	KeyFrame* UpdateCurrentAnimation(Time frameTime);

private:
	void FindCurrentClipRange();

private:
	KeyFrameRange keyframeRange;
	Array<AnimationClip*> clips;
	AnimationClip* currentClip = nullptr;
	AnimationTimeController* currentTimer = nullptr;
	Time::Duration frameRate;
	uint32 currentClipIndex;
	bool playing = true;
	bool pad[3] = { false, false, false };
};