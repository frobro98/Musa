// Copyright 2020, Nathan Blane

#include "Material.hpp"
#include "Texture2D/TextureManager.h"
#include "Texture2D/Texture.h"
#include "ShaderObjects/UnlitShading.hpp"
#include "Graphics/GraphicsInterface.hpp"
#include "Graphics/UniformBuffers.h"
#include "Shader/ShaderResourceManager.hpp"

Material::Material()
{
	vertexShader = &GetShader<UnlitVert>()->GetNativeShader();
	fragmentShader = &GetShader<UnlitFrag>()->GetNativeShader();
	textures[0] = GetTextureManager().FindTexture(TextureManager::DefaultTexture);
	diffuseColor = Color32::Magenta();

	ConfigureMaterialInfo();
}

Material::Material(NativeVertexShader& vertShader, NativeFragmentShader& fragShader, const char* textureName, const Color32& color)
	: diffuseColor(color),
	vertexShader(&vertShader),
	fragmentShader(&fragShader)
{
	REF_CHECK(vertShader, fragShader, color);

	if (textureName != nullptr)
	{
		textures[0] = GetTextureManager().FindTexture(textureName);
	}
	else
	{
		textures[0] = GetTextureManager().FindTexture(TextureManager::DefaultTexture);
	}

    ConfigureMaterialInfo();
}

Material::Material(NativeVertexShader& vertShader, NativeFragmentShader& fragShader, const Texture* tex, const Color32& color)
	: diffuseColor(color),
	vertexShader(&vertShader),
	fragmentShader(&fragShader)
{
	REF_CHECK(vertShader, fragShader, color);

	textures[0] = tex;

    ConfigureMaterialInfo();
}

Material::Material(ShaderID vertexID, ShaderID fragmentID)
{
	ShaderResource* vertShader = GetShaderResourceManager().FindShaderResource(vertexID);
	Assert(vertShader != nullptr);
	ShaderResource* fragShader = GetShaderResourceManager().FindShaderResource(fragmentID);
	Assert(fragShader != nullptr);

	shader.Initialize(*vertShader, *fragShader);
}

void Material::SetTexture(Texture& tex0)
{
	REF_CHECK(tex0);
	textures[0] = &tex0;
	ConfigureMaterialInfo();
}

void Material::SetColor(const Color32& color)
{
	REF_CHECK(color);
	diffuseColor = color;
	ConfigureMaterialInfo();
}

void Material::SetShadingModel(ShadingModel model)
{
	shadingModel = model;
	ConfigureMaterialInfo();
}

void Material::ConfigureMaterialInfo()
{
	materialRendering->baseColor = diffuseColor;
	materialRendering->baseTexture = textures[0] ? textures[0]->gpuResource.Get() : nullptr;
	materialRendering->vertexShader = vertexShader;
	materialRendering->fragmentShader = fragmentShader;
	materialRendering->shadingModel = shadingModel;
	materialRendering->materialProperties = GetGraphicsInterface().CreateUniformBuffer(sizeof(MaterialProperties));
}
