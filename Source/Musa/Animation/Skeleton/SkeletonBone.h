#pragma once

#include "PCSTree/PCSNode.h"

class Matrix4;
class Model;
struct FrameData;

class SkeletonBone : public PCSNode<SkeletonBone>
{
public:
	SkeletonBone();
	~SkeletonBone();

	void SetIndex(uint32 index);
	void SetDebug(bool debug);
	void SetOrientation(const Matrix4& orientation);

	Matrix4 GetWorld() const;

	void UpdateAnimationTransform(FrameData* boneData);
	// TODO - Get rid of this hack. This is bad and it should feel bad
	void UpdateRootAnimationTransform(FrameData* boneData, const Matrix4& parent);

private:
	Model* debugModel;
	Matrix4* world;
	uint32 boneIndex;
};