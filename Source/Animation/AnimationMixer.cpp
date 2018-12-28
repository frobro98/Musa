
#include "AnimationMixer.h"
#include "KeyFrame.h"
#include "Math/VectorFunctions.h"
#include "Math/QuatFunctions.h"

AnimationMixer::AnimationMixer(uint32 skelBoneCount)
	: resultingFrame(new KeyFrame(skelBoneCount))
{
}

AnimationMixer::~AnimationMixer()
{
	delete resultingFrame;
}

void AnimationMixer::InterpolateAnimationFrames(Time deltaTime, const KeyFrame* frameA, const KeyFrame* frameB)
{
	resultingFrame->frameTime = deltaTime;
	float deltaS = (deltaTime - frameA->frameTime) / (frameB->frameTime - frameA->frameTime);

	uint32 totalBones = resultingFrame->boneCount;
	const FrameData* boneA = frameA->boneFrameData;
	const FrameData* boneB = frameB->boneFrameData;
	FrameData* resultingBone = resultingFrame->boneFrameData;

	for (uint32 i = 0; i < totalBones; ++i)
	{
		Math::Lerp(resultingBone->translation, boneA->translation, boneB->translation, deltaS);
		Math::Slerp(resultingBone->rotation, boneA->rotation, boneB->rotation, deltaS);
		Math::Lerp(resultingBone->scale, boneA->scale, boneB->scale, deltaS);

		++boneA;
		++boneB;
		++resultingBone;
	}
}

KeyFrame* AnimationMixer::GetResultingFrame() const
{
	return resultingFrame;
}
