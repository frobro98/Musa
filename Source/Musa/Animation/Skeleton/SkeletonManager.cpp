
#include "SkeletonManager.h"
#include "Skeleton.h"
#include "SkeletonBone.h"
// TODO - Remove this dependency
#include "Graphics/UniformBuffers.h"

#include "Archiver/SkeletonHeader.h"
#include "PCSTree/PCSNode.h"
#include "PCSTree/PCSTree.h"

#include "DebugOutput.h"

Skeleton* SkeletonManager::CreateSkeleton(uint8* skeletonData, uint32 hash)
{
	Skeleton* skel = FindSkeleton(hash);
	if (skel == nullptr)
	{
		PCSTree<SkeletonBone> boneHierarchy;
		SkeletonHeader* header = reinterpret_cast<SkeletonHeader*>(skeletonData);
		uint32 boneCount = header->boneCount;
		uint8* boneHierarchyData = skeletonData + header->boneHierarchyOffset;
		SingleBoneData* boneData = reinterpret_cast<SingleBoneData*>(boneHierarchyData);

		SkeletonBone* bones = new SkeletonBone[boneCount];
		for (uint32 i = 0; i < boneCount; ++i)
		{
			if (boneData[i].parentIndex < 0)
			{
				bones[i].name = boneData[i].boneName;
				boneHierarchy.Insert(&bones[i], nullptr);
			}
			else
			{
				bones[i].name = boneData[i].boneName;
				int32 parentIndex = boneData[i].parentIndex;
				boneHierarchy.Insert(&bones[i], &bones[parentIndex]);
			}
			bones[i].SetIndex(i);
		}

		Assert(header->boneTableOffset > 0);
		uint8* tableData = skeletonData + header->boneTableOffset;
		uint32 maxElementLength = *(reinterpret_cast<uint32*>(tableData));
		tableData += sizeof(uint32);
		Assert(maxElementLength < BoneHierarchyTable::BoneTableElement::MaxElementLength);

		BoneHierarchyTable table;
		for (uint32 i = 0; i < boneCount; ++i)
		{
			uint32 numParents = *(reinterpret_cast<uint32*>(tableData));
			tableData += sizeof(uint32);
			//table.table[i].numParents = numParents;

			uint32 j;
			for (j = 0; j < numParents; ++j)
			{
				table.table[i].boneParentHierarchy[j] = *(reinterpret_cast<uint32*>(tableData));
				tableData += sizeof(int32);
			}
			for (; j < BoneHierarchyTable::BoneTableElement::MaxElementLength; ++j)
			{
				// Allows there to be an identity calculation in the shader
				table.table[i].boneParentHierarchy[j] = static_cast<uint32>(boneCount);
			}
		}

		BonePoseData* poses = nullptr;
		if (header->bonePoseOffset > 0)
		{
			uint8* poseData = skeletonData + header->bonePoseOffset;

			poses = new BonePoseData[boneCount];
			size_t poseCountBytes = sizeof(BonePoseData) * boneCount;
			Memcpy(poses, poseCountBytes, poseData, poseCountBytes);
		}

		skel = new Skeleton(bones, poses, table, boneCount, hash, std::move(boneHierarchy));

		Instance().AddSkeleton(skel);

	}

	return skel;
}

void SkeletonManager::RemoveSkeleton(Skeleton* skeleton)
{
	UNUSED(skeleton);
}

Skeleton* SkeletonManager::FindSkeleton(uint32 hash)
{
	Skeleton* skel = nullptr;
	for (auto& skeleton : Instance().skeletons)
	{
		if (skeleton->GetHash() == hash)
		{
			skel = skeleton;
			break;
		}
	}

	return skel;
}

void SkeletonManager::AddSkeleton(Skeleton* skeleton)
{
	skeletons.Add(skeleton);
}

SkeletonManager& SkeletonManager::Instance()
{
	static SkeletonManager skManager;
	return skManager;
}
