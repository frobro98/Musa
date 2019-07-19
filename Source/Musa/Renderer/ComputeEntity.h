#pragma once

#include "Graphics/Vulkan/VulkanStorageBuffer.h"
#include "Graphics/Vulkan/VulkanUniformBuffer.h"

class Model;
class VulkanDescriptorSet;

class ComputeEntity
{
public:
	ComputeEntity(const VulkanDevice& device, Model& model);

	ComputeEntity(const ComputeEntity&) = delete;
	ComputeEntity& operator=(const ComputeEntity&) = delete;

	Model* GetCPUModel() { return referenceModel; }

	UniformBuffer& GetKeyframeConstantsBuffer() { return *keyframeConstantsBuffer; }
	UniformBuffer& GetBlendConstantsBuffer() { return *blendKeyframeBuffer; }
	UniformBuffer& GetBlendResultingConstantsBuffer() { return *blendResultConstantsBuffer; }

	StorageBuffer& GetFirstFrameStorage() { return frame0Buffer; }
	StorageBuffer& GetSecondFrameStorage() { return frame1Buffer; }

	StorageBuffer& GetFirstPassBlendResultStorage() { return firstPassResultBuffer; }
	StorageBuffer& GetInterpolatedTransformsStorage() { return interpolatedTransformsBuffer; }
	StorageBuffer& GetBoneMatrixStorage() { return boneMatrixStorageBuffer; }

	StorageBuffer& GetFirstBlendFrameStorage() { return blendFrame0Buffer; }
	StorageBuffer& GetSecondBlendFrameStorage() { return blendFrame1Buffer; }
	StorageBuffer& GetBlendTransformsBuffer() { return blendTransformsBuffer; }

	StorageBuffer& GetHierarchyTableBuffer() { return boneHierarchyTable; }
	StorageBuffer& GetTransformedHierarchy() { return transformedBoneHierarchy; }

	VulkanDescriptorSet* GetFirstPassDescriptorSet() { return firstPassDescriptors; }
	VulkanDescriptorSet* GetProcessingPassDescriptorSet() { return processPassDescriptors; }
	VulkanDescriptorSet* GetFinalPassDescriptorSet() { return finalPassDescriptors; }
	
	VulkanDescriptorSet* GetFirstPassBlendDescriptorSet() { return firstPassBlendDescriptors; }
	VulkanDescriptorSet* GetBlendingDescriptorSet() { return blendingDescriptors; }
	VulkanDescriptorSet* GetBlendingResultDescriptorSet() { return blendingResultDescriptors; }

	void SetFirstPassDescriptorSet(VulkanDescriptorSet& descriptorSet);
	void SetFirstPassBlendDescriptor(VulkanDescriptorSet& descriptorSet);
	void SetProcessingPassDescriptorSet(VulkanDescriptorSet& descriptorSet);
	void SetFinalPassDescriptorSet(VulkanDescriptorSet& descriptorSet);

	void SetBlendingDescriptorSet(VulkanDescriptorSet& descriptorSet);
	void SetBlendingResultDescriptorSet(VulkanDescriptorSet& descriptorSet);

private:
	// First compute shader
	// TODO - implement simple compression and then redo this upload configuration
	StorageBuffer frame0Buffer;
	StorageBuffer frame1Buffer;
	StorageBuffer blendFrame0Buffer;
	StorageBuffer blendFrame1Buffer;

	UniformBuffer* keyframeConstantsBuffer;
	StorageBuffer firstPassResultBuffer;
	StorageBuffer interpolatedTransformsBuffer;

	// Blending
	UniformBuffer* blendKeyframeBuffer;
	UniformBuffer* blendResultConstantsBuffer;
	StorageBuffer blendTransformsBuffer;
	StorageBuffer boneMatrixStorageBuffer;

	// Second compute shader
	StorageBuffer boneHierarchyTable;
	StorageBuffer transformedBoneHierarchy;

	// Other info
	VulkanDescriptorSet* firstPassDescriptors = nullptr;
	VulkanDescriptorSet* firstPassBlendDescriptors = nullptr;
	VulkanDescriptorSet* blendingDescriptors = nullptr;
	VulkanDescriptorSet* blendingResultDescriptors = nullptr;
	VulkanDescriptorSet* processPassDescriptors = nullptr;
	VulkanDescriptorSet* finalPassDescriptors = nullptr;
	Model* referenceModel;
	const VulkanDevice& logicalDevice;
	uint32 pad[3];
};