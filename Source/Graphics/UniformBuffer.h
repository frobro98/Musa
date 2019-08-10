#pragma once

#include "Platform.h"
#include "MemoryUtilities.h"
#include "Math/MathEngine.h"
#include "Color.hpp"

constexpr uint32 MaxBones = 120;

struct TransformationUniformBuffer
{
	Matrix model;
	Matrix view;
	Matrix projection;
};

struct PoseInverseUniformBuffer
{
	Matrix inverses[MaxBones];

	PoseInverseUniformBuffer()
	{
		Memset(inverses, 0, sizeof(Matrix) * MaxBones);
	}
};

struct BoneDeltasUniformBuffer
{
	Matrix deltaTransforms[MaxBones];

	BoneDeltasUniformBuffer()
	{
		Memset(deltaTransforms, 0, sizeof(Matrix) * MaxBones);
	}
};

struct MaterialProperties
{
	Color32 diffuse;
};

struct ViewProperties
{
	Vector4 viewPosition;
};

struct LightProperties
{
	Vector4 position;
	Vector4 direction;
	Color32 color;
	Matrix lightViewTransform;
	Matrix lightProjection;//lightSpaceTransform;
};

struct KeyframeConstanceUniformBuffer
{
	uint32 boneCount;
	float deltaTime;
};

struct BoneHierarchyTable
{
	struct BoneTableElement
	{
		static constexpr uint32 MaxElementLength = 20;
		uint32 boneParentHierarchy[MaxElementLength];
	};

	BoneTableElement table[MaxBones];
};