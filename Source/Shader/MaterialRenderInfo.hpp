// Copyright 2020, Nathan Blane

#pragma once

#include "Color.hpp"
#include "Types/UniquePtr.hpp"
#include "Graphics/GraphicsAPIDefinitions.hpp"
#include "Graphics/GraphicsResourceDefinitions.hpp"

class Material;
struct ShaderResource;

struct MaterialRenderInfo
{
	Color32 baseColor = Color32::White();
	UniquePtr<NativeUniformBuffer> materialProperties;
	NativeTexture* baseTexture = nullptr;
	NativeTexture* normalMap = nullptr;
	ShaderResource* vertexShader = nullptr;
	ShaderResource* fragmentShader = nullptr;
	ShadingModel shadingModel = ShadingModel::Lit;
	uint32 pad[1] = { 0 };

	void UpdateRenderMaterial(const Material& material);
};