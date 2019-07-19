#pragma once

#include "Platform.h"
#include "Containers/DynamicArray.hpp"

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

	DynamicArray<AnimationClip> clips;
};