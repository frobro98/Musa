#pragma once

#include "Platform.h"
#include "Graphics.h"
#include "Renderer/UniformBuffer.h"
#include "Malloc16Align.h"
#include "Containers/Array.h"

class Mesh;
class Matrix;
class Material;
class DrawList;
class SkeletonBone;
class SkeletonInstance;
struct BonePoseData;

class Model
{
	ALIGN16_OPERATORS;
public:
	Model(Mesh* mesh, Material* mat);
	~Model();

	void SetWorld(const Matrix& world);
	void SetBonePose(const Array<BonePoseData>& poses);
	void SetMaterial(Material* mat);
	void SetOwningDrawList(DrawList* drawList);
	void SetActive(bool isActive);
	void SetActiveSkeleton(SkeletonInstance& skel);

	SkeletonInstance* GetSkeleton() { return skeleton; }
	Matrix* GetWorld() const { return world; }
	Material* GetMaterial() const { return material; }
	Mesh* GetMesh() const { return mesh; }

	const Matrix* GetBoneMats() const;
	Array<BonePoseData> GetPoseData() const;

private:
	Matrix boneMats[MaxBones];
	Array<BonePoseData> poseData;
	Matrix* world;
	SkeletonInstance* skeleton = nullptr;
	Mesh* mesh;
	Material* material;
	DrawList* owningDrawList = nullptr;
	bool active = true;
	bool pad[31] = {};
};