
#include "SkeletonBone.h"
#include "Animation/KeyFrame.h"
#include "Model/Model.h"
#include "Model/Mesh.h"
#include "Model/MeshManager.h"
#include "Shader/Material.h"
#include "Math/MathEngine.h"
#include "Model/ModelFactory.h"

SkeletonBone::SkeletonBone()
	: world(new Matrix(IDENTITY))
{
	Mesh* pyramid = MeshManager::FindMesh(Mesh::PyramidName);
	Material* mat = new Material;
	debugModel = ModelFactory::CreateModel(pyramid, mat);
}

SkeletonBone::~SkeletonBone()
{
}

void SkeletonBone::SetIndex(uint32 index)
{
	boneIndex = index;
}

void SkeletonBone::SetDebug(bool debug)
{
	debugModel->SetActive(debug);
}

void SkeletonBone::SetOrientation(const Matrix& orientation)
{
	debugModel->SetWorld(orientation);
}

Matrix SkeletonBone::GetWorld() const
{
	return *world;
}

void SkeletonBone::UpdateAnimationTransform(FrameData* boneData)
{
	assert(parent != nullptr);

	Matrix parentWorld = *parent->world;

	FrameData* correctData = &boneData[boneIndex];
	Matrix trans = Matrix(TRANS, correctData->translation);
	Quat rotation = correctData->rotation;
	Matrix scale = Matrix(SCALE, correctData->scale);

	*world = scale * rotation * trans * parentWorld;

	debugModel->SetWorld(*world);
}

void SkeletonBone::UpdateRootAnimationTransform(FrameData* boneData, const Matrix& par)
{
	Matrix parentWorld = par;

	FrameData* correctData = &boneData[boneIndex];
	Matrix trans = Matrix(TRANS, correctData->translation);
	Quat rotation = correctData->rotation;
	Matrix scale = Matrix(SCALE, correctData->scale);

	*world = scale * rotation * trans * parentWorld;

	debugModel->SetWorld(*world);
}
