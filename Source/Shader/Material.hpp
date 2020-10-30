// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/UniquePtr.hpp"
#include "BasicTypes/Color.hpp"
#include "Graphics/GraphicsAPIDefinitions.hpp"
#include "Graphics/UniformBuffers.h"
#include "Shader/MaterialRenderDescription.hpp"
#include "Shader/ShaderAPI.hpp"
#include "Shader/MaterialShader.hpp"
#include "Shader/ShaderID.hpp"
#include "Shader/MaterialResourceHandles.hpp"

class Texture;
struct TextureCube;
struct MaterialRenderDescription;
class ShaderProgram;
class Matrix4;

// A Material is essentially all the data that's required for a render to take place.
// This information can be pretty trivial to set i.e. blending, raster, DepthSten, etc.,
// but there is other information that would be a bit more difficult to manage. This data
// is essentially tied not necessarily to the Material itself, but to the shader. The
// Material has to cater to what data the shader supports. To do so isn't supported,
// but will be brainstormed now
//
// Essentially, the information that actually will show up in the render, like textures,
// color, or PBR attributes, all are shader specified. The backing data, for obvious
// reasons, needs to live within the Material itself, but to actually be able to set
// and use the data is dictated by the shaders themselves.
//
// Because the backing data lives within the Material and is set based on shader
// requirements, there needs to be validation. This would be in this kind of form:
//		1) If you attempt to bind to a descriptor that doesn't exist, that's an error
//		2) If you don't bind to all of the descriptors that are necessary, that's an
//		   error as well.
//		

class SHADER_API Material
{
public:
	Material() = default;
	Material(ShaderID vertexID, ShaderID fragmentID);

	forceinline UniformBufferShaderConstant GetUniformBufferConstant(const tchar* resName) const
	{
		u16 index = shader.GetUniformBufferResourceIndex(resName);
		return UniformBufferShaderConstant{ index };
	}

	forceinline TextureSamplerShaderConstant GetTextureSamplerConstant(const tchar* resName) const
	{
		u16 index = shader.GetTextureSamplerResourceIndex(resName);
		return TextureSamplerShaderConstant{ index };
	}

	forceinline void SetUniformBufferResource(UniformBufferShaderConstant res, NativeUniformBuffer& uniformBuffer)
	{
		REF_CHECK(uniformBuffer);
		shader.SetUniformBufferResource(res.resourceIndex, uniformBuffer);
	}

	forceinline void SetTextureSamplerResource(TextureSamplerShaderConstant res, NativeTexture& texture, NativeSampler& sampler)
	{
		shader.SetTextureSamplerResource(res.resourceIndex, texture, sampler);
	}

	forceinline void SetColor(Color32 color)
	{
		materialProperties.diffuse = color;
		renderDescDirty = true;
	}

	forceinline void SetShadingModel(ShadingModel model)
	{
		shadingModel = model;
		renderDescDirty = true;
	}

	forceinline void SetBlendMode(BlendMode mode)
	{
		blendMode = mode;
		renderDescDirty = true;
	}

	forceinline void SetFillMode(FillMode mode)
	{
		fillMode = mode;
		renderDescDirty = true;
	}

	forceinline void SetCullingMode(CullingMode mode)
	{
		cullMode = mode;
		renderDescDirty = true;
	}

	MaterialRenderDescription& GetRenderDescription();

private:


private:
	UniquePtr<MaterialRenderDescription> renderDescription;

	// TODO - Keeping a uniform buffer in the material itself doesn't necessarily make sense
	UniquePtr<NativeUniformBuffer> materialPropsBuffer;
	MaterialProperties materialProperties;
	UniformBufferShaderConstant materialPropsConstant;

	// Pipeline properties
	MaterialShader shader;
	CullingMode cullMode = CullingMode::Back;
	FillMode fillMode = FillMode::Full;
	BlendMode blendMode = BlendMode::Opaque;
	ShadingModel shadingModel = ShadingModel::Lit;
	bool renderDescDirty = false;
};