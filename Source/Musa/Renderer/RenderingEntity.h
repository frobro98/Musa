#pragma once

#include "Graphics.h"
#include "Graphics/Vulkan/VulkanVertexBuffer.h"
#include "Graphics/Vulkan/VulkanIndexBuffer.h"
#include "Graphics/Vulkan/VulkanUniformBuffer.h"

class Renderer;
class VulkanDescriptorSet;
struct MeshRenderInfo;

// TODO - This should be an abstract class of many different rendering entities later on
class RenderingEntity
{
public:
	RenderingEntity(Renderer* renderer_, MeshRenderInfo& info);
	~RenderingEntity();

	RenderingEntity(const RenderingEntity&) = delete;
	RenderingEntity& operator=(const RenderingEntity&) = delete;

	void SetRenderer(Renderer* renderer_) { renderer = renderer_; }

	MeshRenderInfo* GetCPUInfo() { return meshInfo; }
	VertexBuffer* GetVertexBuffer() { return vertBuffer; }
	WeightsBuffer* GetWeightsBuffer() { return weightsBuffer; }
	IndexBuffer* GetIndexBuffer() { return indexBuffer; }
	UniformBuffer* GetViewPropertiesBuffer() { return viewPropsUBO; }
	UniformBuffer* GetTransformBuffer() { return transformUBO; }
	UniformBuffer* GetMaterialPropsBuffer() { return materialPropsUBO; }
	UniformBuffer* GetPoseInverseBuffer() { return poseInversesUBO; }
	UniformBuffer* GetBoneDeltaBuffer() { return boneDeltasUBO; }
	VulkanTexture* GetNativeTexture() { return nativeTexture; }
	VulkanTexture* GetNativeTexture1() { return nativeTexture1; }
	VulkanTexture* GetNormalMap() { return normalMap; }
	VulkanDescriptorSet* GetDescriptorSet() const { return descriptorSet; }

private:
	void SetupTextures();

private:
	VulkanTexture* nativeTexture;
	// TODO - get rid of hack!
	VulkanTexture* nativeTexture1;
	VulkanTexture* normalMap;
	UniformBuffer* transformUBO; // For transformations
	UniformBuffer* materialPropsUBO; // For materials
	UniformBuffer* viewPropsUBO; // For view properties
	UniformBuffer* poseInversesUBO;
	UniformBuffer* boneDeltasUBO;
	VulkanDescriptorSet* descriptorSet = nullptr;
	VertexBuffer* vertBuffer;
	WeightsBuffer* weightsBuffer;
	IndexBuffer* indexBuffer;
	Renderer* renderer;
	MeshRenderInfo* meshInfo;
	uint32 pad[1] = { 0 };
};