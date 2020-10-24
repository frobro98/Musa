// Copyright 2020, Nathan Blane

#pragma once

#include "Shader/ShaderTables.hpp"
class ShaderResource;

enum MaterialShaderMask
{
	VertexMask = 0x01,
	FragmentMask = 0x02,
	GeomertyMask = 0x04,
	TessControlMask = 0x08,
	TessEvalMask = 0x10
};

struct MaterialResourceTable
{
	ShaderResourceTable resourceStores[ShaderStage::GfxStageCount];
};
class MaterialShader
{
public:
	MaterialShader() = default;
	void Initialize(ShaderResource& vertexShader, ShaderResource& fragmentShader);

	// This is where the data storage will be held for the shaders. We will have to send this information
	// over to the render pipeline, so actually destroying the storage will be a little bit tricky, but it
	// might not be that crazy to implement

private:
	void ConstructResourceTable();

private:
	ShaderResource* shaderResources[ShaderStage::GfxStageCount];
	MaterialResourceTable resourceTable;
};