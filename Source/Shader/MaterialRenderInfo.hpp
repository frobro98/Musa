// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Color.hpp"
#include "BasicTypes/UniquePtr.hpp"
#include "Graphics/GraphicsAPIDefinitions.hpp"
#include "Graphics/GraphicsResourceDefinitions.hpp"
#include "Shader/ShaderAPI.hpp"

class Material;
struct NativeVertexShader;
struct NativeFragmentShader;

struct SHADER_API MaterialRenderInfo
{
	Color32 baseColor = Color32::White();
	UniquePtr<NativeUniformBuffer> materialProperties;
	NativeTexture* baseTexture = nullptr;
	NativeTexture* normalMap = nullptr;
	NativeVertexShader* vertexShader = nullptr;
	NativeFragmentShader* fragmentShader = nullptr;
	ShadingModel shadingModel = ShadingModel::Lit;

	void UpdateRenderMaterial(const Material& material);
};