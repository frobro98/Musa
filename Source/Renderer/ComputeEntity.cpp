#include "ComputeEntity.h"
#include "Math/MathEngine.h"
#include "Model/Model.h"
#include "Animation/Skeleton/SkeletonInstance.h"
#include "Archiver/SkeletonHeader.h"
#include "UniformBuffer.h"

ComputeEntity::ComputeEntity(Model& model)
	: referenceModel(&model)
{
	SkeletonInstance& skel = *model.GetSkeleton();
	// First pass initialization
	uint32 boneCount = skel.GetBoneCount();

	frame0Buffer.Initialize(sizeof(FrameData) * boneCount);
	frame1Buffer.Initialize(sizeof(FrameData) * boneCount);

	blendFrame0Buffer.Initialize(sizeof(FrameData) * boneCount);
	blendFrame1Buffer.Initialize(sizeof(FrameData) * boneCount);
	blendTransformsBuffer.Initialize(sizeof(uint32) + sizeof(Matrix) * boneCount);

	keyframeConstantsBuffer.Initialize(sizeof(KeyframeConstanceUniformBuffer));
	blendKeyframeBuffer.Initialize(sizeof(KeyframeConstanceUniformBuffer));
	blendResultConstantsBuffer.Initialize(sizeof(KeyframeConstanceUniformBuffer));

	firstPassResultBuffer.Initialize(sizeof(uint32) + sizeof(Matrix) * boneCount);
	interpolatedTransformsBuffer.Initialize(sizeof(uint32) + sizeof(Matrix) * boneCount);
	boneMatrixStorageBuffer.Initialize(sizeof(Matrix) * (boneCount+1));

	// Final pass initialization
	boneHierarchyTable.Initialize(sizeof(BoneHierarchyTable));
	transformedBoneHierarchy.Initialize(sizeof(Matrix) * boneCount);
}

void ComputeEntity::SetFirstPassDescriptorSet(VulkanDescriptorSet& descriptorSet)
{
	firstPassDescriptors = &descriptorSet;
}

void ComputeEntity::SetFirstPassBlendDescriptor(VulkanDescriptorSet& descriptorSet)
{
	firstPassBlendDescriptors = &descriptorSet;
}

void ComputeEntity::SetProcessingPassDescriptorSet(VulkanDescriptorSet& descriptorSet)
{
	processPassDescriptors = &descriptorSet;
}

void ComputeEntity::SetFinalPassDescriptorSet(VulkanDescriptorSet& descriptorSet)
{
	finalPassDescriptors = &descriptorSet;
}

void ComputeEntity::SetBlendingDescriptorSet(VulkanDescriptorSet& descriptorSet)
{
	blendingDescriptors = &descriptorSet;
}

void ComputeEntity::SetBlendingResultDescriptorSet(VulkanDescriptorSet& descriptorSet)
{
	blendingResultDescriptors = &descriptorSet;
}
