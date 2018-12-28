#pragma once

#include "Graphics.h"
#include "Vulkan/VulkanVertexBuffer.h"
#include "Vulkan/VulkanIndexBuffer.h"
#include "Vulkan/VulkanUniformBuffer.h"

class Model;
class Renderer;
class VulkanDescriptorSet;

// TODO - This should be an abstract class of many different rendering entities later on
class RenderingEntity
{
public:
	RenderingEntity(Renderer* renderer_, Model* model);
	virtual ~RenderingEntity();

	RenderingEntity(const RenderingEntity&) = delete;
	RenderingEntity& operator=(const RenderingEntity&) = delete;

	void SetRenderer(Renderer* renderer_) { renderer = renderer_; }

	Model* GetCPUModel() { return referencedModel; }
	VertexBuffer* GetVertexBuffer() { return &vertBuffer; }
	WeightsBuffer* GetWeightsBuffer() { return &weightsBuffer; }
	IndexBuffer* GetIndexBuffer() { return &indexBuffer; }
	UniformBuffer* GetViewPropertiesBuffer() { return &viewPropsUBO; }
	UniformBuffer* GetTransformBuffer() { return &transformUBO; }
	UniformBuffer* GetMaterialPropsBuffer() { return &materialPropsUBO; }
	UniformBuffer* GetPoseInverseBuffer() { return &poseInversesUBO; }
	UniformBuffer* GetBoneDeltaBuffer() { return &boneDeltasUBO; }
	VulkanTexture* GetNativeTexture() { return &nativeTexture; }
	VulkanTexture* GetNativeTexture1() { return &nativeTexture1; }
	uint32 GetNumIndicies() const { return numMeshIndices; }
	VulkanDescriptorSet* GetDescriptorSet() const { return descriptorSet; }

	void SetDescriptorSet(VulkanDescriptorSet* descriptorSet);

private:
	void SetupTextures();

private:
	VulkanTexture nativeTexture;
	// TODO - get rid of hack!
	VulkanTexture nativeTexture1;
	VertexBuffer vertBuffer;
	WeightsBuffer weightsBuffer;
	IndexBuffer indexBuffer;
	UniformBuffer transformUBO; // For transformations
	UniformBuffer materialPropsUBO; // For materials
	UniformBuffer viewPropsUBO; // For view properties
	UniformBuffer poseInversesUBO;
	UniformBuffer boneDeltasUBO;
	VulkanDescriptorSet* descriptorSet;
	Renderer* renderer;
	Model* referencedModel;
	uint32 numMeshIndices;
};