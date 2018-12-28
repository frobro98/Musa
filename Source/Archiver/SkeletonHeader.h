#pragma once

// TODO - Move this file out of the Archiver project

#include "Platform.h"
#include "Math/MathEngine.h"

constexpr const uint32 BoneNameLength = 64;

struct SkeletonHeader
{
	uint32 boneCount = 0;
	uint32 boneHierarchyOffset = 0;
	uint32 boneTableOffset = 0;
	uint32 bonePoseOffset = 0;
};

struct SingleBoneData
{
	tchar boneName[BoneNameLength];
	int32 parentIndex;
};

struct BonePoseData
{
	Matrix inversePoseMatrix;
};

