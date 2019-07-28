#pragma once

#include "Color.hpp"
#include "Graphics/GraphicsAPIDefinitions.hpp"

#include "Graphics/Vulkan/VulkanUniformBuffer.h"

struct VulkanTexture;
class Material;
struct ShaderResource;

struct MaterialRenderInfo
{
	Color32 baseColor = Color32::White();
	UniformBuffer* materialProperties;
	VulkanTexture* baseTexture = nullptr;
	VulkanTexture* normalMap = nullptr;
	ShaderResource* vertexShader = nullptr;
	ShaderResource* fragmentShader = nullptr;
	ShadingModel shadingModel = ShadingModel::Lit;
	uint32 pad[1] = { 0 };

	void UpdateRenderMaterial(const Material& material);
};