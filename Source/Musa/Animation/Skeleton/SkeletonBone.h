#pragma once

#include "PCSTree/PCSNode.h"

class Matrix;
class Model;
struct FrameData;

class SkeletonBone : public PCSNode<SkeletonBone>
{
public:
	SkeletonBone();
	~SkeletonBone();

	void SetIndex(uint32 index);
	void SetDebug(bool debug);
	void SetOrientation(const Matrix& orientation);

	Matrix GetWorld() const;

	void UpdateAnimationTransform(FrameData* boneData);
	// TODO - Get rid of this hack. This is bad and it should feel bad
	void UpdateRootAnimationTransform(FrameData* boneData, const Matrix& parent);

private:
	Model* debugModel;
	Matrix* world;
	uint32 boneIndex;
};