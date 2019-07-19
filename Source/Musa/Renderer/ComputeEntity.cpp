#include "ComputeEntity.h"
#include "Math/MathEngine.h"
#include "Model/Model.h"
//#include "Animation/Skeleton/SkeletonInstance.h"
#include "Archiver/SkeletonHeader.h"

// TODO - Remove Vulkan references
#include "Graphics/UniformBuffer.h"
#include "Graphics/Vulkan/VulkanMemory.h"

// TODO - There is a buffer leak now. This needs to be fixed!

ComputeEntity::ComputeEntity(const VulkanDevice& device, Model& model)
	: referenceModel(&model),
	logicalDevice(device)
{
	SkeletonInstance& skel = *model.GetSkeleton();
	// First pass initialization
	uint32 boneCount = skel.GetBoneCount();

	frame0Buffer.Initialize(device, sizeof(FrameData) * boneCount);
	frame1Buffer.Initialize(device, sizeof(FrameData) * boneCount);

	blendFrame0Buffer.Initialize(device, sizeof(FrameData) * boneCount);
	blendFrame1Buffer.Initialize(device, sizeof(FrameData) * boneCount);
	blendTransformsBuffer.Initialize(device, sizeof(uint32) + sizeof(Matrix) * boneCount);

	keyframeConstantsBuffer = VulkanMemory::CreateUniformBuffer(sizeof(KeyframeConstanceUniformBuffer));
	blendKeyframeBuffer = VulkanMemory::CreateUniformBuffer(sizeof(KeyframeConstanceUniformBuffer));
	blendResultConstantsBuffer = VulkanMemory::CreateUniformBuffer(sizeof(KeyframeConstanceUniformBuffer));

	firstPassResultBuffer.Initialize(device, sizeof(uint32) + sizeof(Matrix) * boneCount);
	interpolatedTransformsBuffer.Initialize(device, sizeof(uint32) + sizeof(Matrix) * boneCount);
	boneMatrixStorageBuffer.Initialize(device, sizeof(Matrix) * (boneCount+1));

	// Final pass initialization
	boneHierarchyTable.Initialize(device, sizeof(BoneHierarchyTable));
	transformedBoneHierarchy.Initialize(device, sizeof(Matrix) * boneCount);
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
