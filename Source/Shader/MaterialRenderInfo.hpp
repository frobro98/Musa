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
	UniquePtr<NativeUniformBuffer> materialProperties;
	Color32 baseColor = Color32::White();
	NativeTexture* baseTexture = nullptr;
	NativeVertexShader* vertexShader = nullptr;
	NativeFragmentShader* fragmentShader = nullptr;
	ShadingModel shadingModel = ShadingModel::Lit;
	CullingMode cullMode = CullingMode::Back;
	FillMode fillMode = FillMode::Full;
	BlendMode blendMode = BlendMode::Opaque;
};