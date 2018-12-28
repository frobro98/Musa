#pragma once

#include "Platform.h"
#include "Containers/Array.h"

class AnimationClip;

class AnimationClipFactory
{
public:

	static AnimationClip* CreateAnimationClip(uint8* animationData);
	static void DestroyAnimationClip(AnimationClip* clip);

private:
	AnimationClipFactory() = default;
	~AnimationClipFactory();

	static AnimationClipFactory& Instance();

private:
	static AnimationClipFactory* cfInstance;

	Array<AnimationClip> clips;
};