#pragma once

#include "Platform.h"
#include "Containers/DynamicArray.hpp"
#include "PCSTree/PCSTree.h"
#include "Animation/Skeleton/SkeletonBone.h"
#include "Time/Time.h"
#include "Animation/AnimationController.h"
// TODO - remove this dependency
#include "Renderer/UniformBuffer.h"


class AnimationClip;
struct BonePoseData;
class Matrix;

class Skeleton
{
public:
	Skeleton(SkeletonBone* boneList, BonePoseData* poseList, BoneHierarchyTable& table, uint32 boneCount, uint32 skeletonHash, PCSTree<SkeletonBone>&& hierarchy);
	~Skeleton();

	Skeleton(const Skeleton&) = delete;
	Skeleton& operator=(const Skeleton&) = delete;

	KeyFrameRange GetFrameRange() const { return controller->GetFrameRange(); }
	SkeletonBone* GetSkeletonBoneList() const { return bones; }
	uint32 GetBoneCount() const { return boneCount; }
	DynamicArray<BonePoseData> GetPose() const { return poseMatrices; }
	const BoneHierarchyTable& GetHierarchyTable() const { return hierarchyTable; }

	uint32 GetHash() const { return skeletonHash; }

	bool IsAnimationDirty() const;

	void SetWorld(const Matrix& m);
	void AddAnimation(AnimationClip* clip);
	void SetDebug(bool debug);
	void Update(Time frameTime);

	AnimationController* GetController() const;

private:

	// TODO - These pose functions should probably go into the bone class
	void SetDebugBonePoses();
	void SetDebugBonePose(SkeletonBone* bone);

private:
	const BoneHierarchyTable hierarchyTable;
	DynamicArray<BonePoseData> poseMatrices;
	PCSTree<SkeletonBone> boneHierarchy;
	SkeletonBone* bones;
	Matrix* skeletonWorld;
	AnimationController* controller = nullptr;
	uint32 boneCount;
	uint32 skeletonHash;
	bool isDebug;
	bool pad[3];

};