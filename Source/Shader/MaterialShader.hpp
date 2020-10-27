// Copyright 2020, Nathan Blane

#pragma once

#include "Shader/ShaderTables.hpp"
class ShaderResource;

// Describes where this resource is used
enum MaterialResourceUsageMask
{
	VertexMask = 0x01,
	FragmentMask = 0x02,
	GeomertyMask = 0x04,
	TessControlMask = 0x08,
	TessEvalMask = 0x10
};
using MaterialResourceUsage = u8;

struct MaterialResourceDesc
{
	// TODO - This should just be a fixed size character array/hashed string
	String name;
	ShaderResourceType type;
	u16 resourceIndex = 0;
	u16 bindIndex = 0;
	u16 size = 0;
	MaterialResourceUsage usage = 0;
};

// This is the data organized per material, not per shader
struct MaterialResourceTable
{
	// Array of all bindings, with index into specific resource array
	DynamicArray<MaterialResourceDesc> resourceBindings;
	DynamicArray<NativeUniformBuffer*> uniformBufferStorage;
	DynamicArray<NativeTexture*> textureStorage;
	DynamicArray<NativeSampler*> samplerStorage;
};

class MaterialShader
{
public:
	MaterialShader() = default;
	void Initialize(ShaderResource& vertexShader, ShaderResource& fragmentShader);

	u16 GetUniformBufferResourceIndex(const tchar* ubName) const;
	u16 GetTextureSamplerResourceIndex(const tchar* texName) const;

	void SetUniformBufferResource(u16 resourceIndex, NativeUniformBuffer& uniformBuffer);
	void SetTextureSamplerResource(u16 resourceIndex, NativeTexture& texture, NativeSampler& sampler);

private:
	void ConstructResourceTable();

private:
	MaterialResourceTable resourceTable;
	ShaderResource* shaderResources[ShaderStage::GfxStageCount];
};