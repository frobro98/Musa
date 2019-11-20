#pragma once

#include "Platform.h"
#include "MemoryUtilities.h"
#include "Math/MathEngine.h"
#include "Color.hpp"

constexpr uint32 MaxBones = 120;

struct TransformationUniformBuffer
{
	Matrix4 model;
};

struct ViewPropertiesBuffer
{
	Matrix4 viewTransform;
	Matrix4 projectionTransform;
	Vector4 viewPosition;
};

struct PoseInverseUniformBuffer
{
	Matrix4 inverses[MaxBones];

	PoseInverseUniformBuffer()
	{
		Memset(inverses, 0, sizeof(Matrix4) * MaxBones);
	}
};

struct BoneDeltasUniformBuffer
{
	Matrix4 deltaTransforms[MaxBones];

	BoneDeltasUniformBuffer()
	{
		Memset(deltaTransforms, 0, sizeof(Matrix4) * MaxBones);
	}
};

struct MaterialProperties
{
	Color32 diffuse;
};

struct LightProperties
{
	Vector4 position;
	Vector4 direction;
	Color32 color;
	Matrix4 lightViewTransform;
	Matrix4 lightProjection;//lightSpaceTransform;
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