
#include "Skeleton.h"
#include "SkeletonBone.h"
#include "Animation/Clip.h"
#include "Animation/AnimationController.h"
#include "Math/MathConstants.h"
#include "Archiver/SkeletonHeader.h"

Skeleton::Skeleton(SkeletonBone* boneList, BonePoseData* poseList, BoneHierarchyTable& table, uint32 count, uint32 hash, PCSTree<SkeletonBone>&& hierarchy)
	: hierarchyTable(table),
	poseMatrices(poseList, count),
	boneHierarchy(std::move(hierarchy)),
	bones(boneList),
	skeletonWorld(new Matrix(IDENTITY)),
	boneCount(count),
	skeletonHash(hash),
	isDebug(true)
{
	assert(poseList != nullptr);
	delete[] poseList;
}

Skeleton::~Skeleton()
{
	delete[] bones;
}

void Skeleton::SetWorld(const Matrix& m)
{
	*skeletonWorld = m;
}

void Skeleton::AddAnimation(AnimationClip* clip)
{
	assert(clip->GetSkeletonHash() == skeletonHash);
	if (controller == nullptr)
	{
		controller = new AnimationController(clip->GetFrameRate());
	}

	controller->AddClip(clip);
}

void Skeleton::Update(Time frameTime)
{
	controller->UpdateCurrentAnimation(frameTime);
}

AnimationController * Skeleton::GetController() const
{
	return controller;
}

void Skeleton::SetDebug(bool debug)
{
	PCSTree<SkeletonBone>::ForwardIterator iter(boneHierarchy.GetRoot());
	while (!iter.IsDone())
	{
		SkeletonBone* bone = *iter;
		assert(bone != nullptr);
		bone->SetDebug(debug);

		++iter;
	}

}

void Skeleton::SetDebugBonePoses()
{
	PCSTree<SkeletonBone>::ForwardIterator iter(boneHierarchy.GetRoot());
	while (!iter.IsDone())
	{
		SkeletonBone* bone = dynamic_cast<SkeletonBone*>(*iter);
		assert(bone != nullptr);
		
		SetDebugBonePose(bone);

		++iter;
	}
}

void Skeleton::SetDebugBonePose(SkeletonBone* bone)
{
	assert(bone != nullptr);

	const SkeletonBone* parentBone = nullptr;
	if (bone->parent != nullptr)
	{
		parentBone = bone->parent;
		assert(parentBone);
	}

	if (parentBone != nullptr)
	{
		Vector start(0, 0, 0);

		Vector initialParentPoint = start * parentBone->GetWorld();
		Vector initialChildPoint = start * bone->GetWorld();

		// Get the direction to the parent bone
		Vector direction = initialParentPoint - initialChildPoint;

		float mag = direction.Magnitude();

		Vector up(0, 1, 0);
		up = up * parentBone->GetWorld();

		const float boneWidth = 8.f;

		Matrix scale(SCALE, boneWidth, mag, boneWidth);
		Quat rot(ROT_ORIENT, direction.GetNormalized(), up);
		Quat rotX(ROT_X, Math::PiOver2);
		Matrix trans(TRANS, initialChildPoint);

		Matrix orientation = scale * rotX* rot * trans;

		bone->SetOrientation(orientation);
	}
}
