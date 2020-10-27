// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/UniquePtr.hpp"
#include "BasicTypes/Color.hpp"
#include "Graphics/GraphicsAPIDefinitions.hpp"
#include "Shader/MaterialRenderInfo.hpp"
#include "Shader/ShaderAPI.hpp"
#include "Shader/MaterialShader.hpp"
#include "Shader/ShaderID.hpp"
#include "Shader/MaterialResourceHandles.hpp"

struct Texture;
struct TextureCube;
struct MaterialRenderInfo;
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

constexpr u32 MaxTexturesPerMaterial = 4;

class SHADER_API Material
{
public:
	Material();
	Material(NativeVertexShader& vertShader, NativeFragmentShader& fragShader, const char* textureName, const Color32& color);
	Material(NativeVertexShader& vertShader, NativeFragmentShader& fragShader, const Texture* tex, const Color32& color);
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

	void SetUniformBufferResource(UniformBufferShaderConstant res, NativeUniformBuffer& uniformBuffer)
	{
		REF_CHECK(uniformBuffer);
		shader.SetUniformBufferResource(res.resourceIndex, uniformBuffer);
	}

	void SetTextureSamplerResource(TextureSamplerShaderConstant res, NativeTexture& texture, NativeSampler& sampler)
	{
		shader.SetTextureSamplerResource(res.resourceIndex, texture, sampler);
	}

	void SetTexture(Texture& texture);
	void SetColor(const Color32& color);
	void SetShadingModel(ShadingModel model);

	MaterialRenderInfo& GetMaterialRenderInfo() const { return *materialRendering; }

private:
	// TODO - This creation method must be refactored to be a better way of creation!
	void ConfigureMaterialInfo();

private:
	Color32 diffuseColor;
// 	float32 roughness = 0.f;
// 	float32 metallic = 0.f;

	UniquePtr<MaterialRenderInfo> materialRendering = new MaterialRenderInfo;
	const Texture* textures[MaxTexturesPerMaterial];

	MaterialShader shader;
	NativeVertexShader* vertexShader;
	NativeFragmentShader* fragmentShader;
	CullingMode cullMode = CullingMode::Back;
	FillMode fillMode = FillMode::Full;
	BlendMode blendMode = BlendMode::Opaque;
	ShadingModel shadingModel = ShadingModel::Lit;
};