
#include "Model.h"
#include "Platform.h"
#include "Shader/Material.h"
#include "Mesh.h"
#include "Renderer/DrawList.h"
#include "Animation/Skeleton/SkeletonBone.h"
#include "Animation/Skeleton/Skeleton.h"
#include "Archiver/SkeletonHeader.h"

Model::Model(Mesh * modelMesh, Material * mat)
	: world(new Matrix),
	mesh(modelMesh),
	material(mat)
{
	Memset(boneMats, 0, sizeof(Matrix) * MaxBones);
}

Model::~Model()
{
	owningDrawList->RemoveFromRenderList(this);
	material = nullptr;
	mesh = nullptr;
}

void Model::SetWorld(const Matrix& worldMat)
{
	*world = worldMat;
}

void Model::SetBonePose(const Array<BonePoseData>& poses)
{
	poseData = poses;
}

void Model::SetMaterial(Material* mat)
{
	material = mat;
}

void Model::SetOwningDrawList(DrawList* drawList)
{
	owningDrawList = drawList;
}

void Model::SetActive(bool isActive)
{
	if (isActive != active)
	{
		if (isActive)
		{
			owningDrawList->AddToRenderList(this);
		}
		else
		{
			owningDrawList->RemoveFromRenderList(this);
		}

		active = isActive;
	}
}

void Model::SetActiveSkeleton(SkeletonInstance& skel)
{
	skeleton = &skel;
}

const Matrix* Model::GetBoneMats() const
{
	return boneMats;
}

Array<BonePoseData> Model::GetPoseData() const
{
	return poseData;
}
