#pragma once

#include "Platform.h"
#include "Graphics.h"
#include "Graphics/UniformBuffer.h"
#include "Malloc16Align.h"
#include "Containers/DynamicArray.hpp"

class Mesh;
class Matrix;
class Material;
class Scene;
class SkeletonBone;
class SkeletonInstance;
struct BonePoseData;
struct MeshRenderInfo;

class Model
{
public:
	Model(Mesh* mesh, Material* mat);
	~Model();

	void SetWorld(const Matrix& world);
	void SetBonePose(const DynamicArray<BonePoseData>& poses);
	void SetMaterial(Material* mat);
	void SetScene(Scene& scene);
	void SetActive(bool isActive);
	void SetActiveSkeleton(SkeletonInstance& skel);

	SkeletonInstance* GetSkeleton() { return skeleton; }
	const Matrix& GetWorld() const { return world; }
	Material* GetMaterial() const { return material; }
	Mesh* GetMesh() const { return mesh; }

	const Matrix* GetBoneMats() const;
	DynamicArray<BonePoseData> GetPoseData() const;

	MeshRenderInfo& GetRenderInfo() const { return *renderInfo; }

private:
	void SetupMeshRenderInfo();

private:
	Matrix boneMats[MaxBones];
	Matrix world;
	DynamicArray<BonePoseData> poseData;
	SkeletonInstance* skeleton = nullptr;
	Mesh* mesh;
	Material* material;
	MeshRenderInfo* renderInfo = nullptr;
	Scene* scene = nullptr;
	bool active = true;
	bool pad[31] = {};
};