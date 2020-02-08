#include "Material.h"
#include "Graphics.h"
#include "Texture2D/TextureManager.h"
#include "Texture2D/Texture.h"
#include "ShaderObjects/UnlitShading.hpp"
#include "Graphics/GraphicsInterface.hpp"
#include "Graphics/UniformBuffers.h"

Material::Material()
{
	vertexShader = &GetShader<UnlitVert>()->GetNativeShader();
	fragmentShader = &GetShader<UnlitFrag>()->GetNativeShader();
	texture0 = GetTextureManager().FindTexture(TextureManager::DefaultTexture);
	diffuseColor = Color32::Magenta();

	ConfigureMaterialInfo();
}

Material::Material(ShaderResource& vertShader, ShaderResource& fragShader, const char* textureName, const Color32& color)
	: diffuseColor(color),
	vertexShader(&vertShader),
	fragmentShader(&fragShader)
{
	if (textureName != nullptr)
	{
		texture0 = GetTextureManager().FindTexture(textureName);
	}
	else
	{
		texture0 = GetTextureManager().FindTexture(TextureManager::DefaultTexture);
	}

    ConfigureMaterialInfo();
}

Material::Material(ShaderResource & vertShader, ShaderResource & fragShader, const Texture* tex, const Color32 & color)
	: diffuseColor(color),
	vertexShader(&vertShader),
	fragmentShader(&fragShader)
{
	texture0 = tex;

    ConfigureMaterialInfo();
}

// Material::Material(const tchar* vertShaderName, const tchar* fragShaderName, const char * textureName, const Color32& color)
// {
// 	Assert(vertShaderName);
// 	Assert(fragShaderName);
// 	//vertexShader = GetShaderManager().CreateShader(vertShaderName, ShaderStage::Vertex);
// 	//fragmentShader = GetShaderManager().CreateShader(fragShaderName, ShaderStage::Fragment);
// 
// 	if (textureName != nullptr)
// 	{
// 		texture0 = GetTextureManager().FindTexture(textureName);
// 	}
// 	else
// 	{
// 		texture0 = GetTextureManager().FindTexture(TextureManager::DefaultTexture);
// 	}
// 	diffuseColor = color;
// 
// 	ConfigureMaterialInfo();
// }

void Material::EnableWireframe()
{
	fillMode = FillMode::Wireframe;
}

void Material::DisableWireframe()
{
	fillMode = FillMode::Full;
}

void Material::SetTexture0(Texture* tex0)
{
	texture0 = tex0;
	ConfigureMaterialInfo();
}

void Material::SetTexture1(Texture* tex1)
{
	texture1 = tex1;
	ConfigureMaterialInfo();
}

void Material::SetNormalMap(Texture* normMap)
{
	normalMap = normMap;
	ConfigureMaterialInfo();
}

void Material::SetColor(const Color32& color)
{
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
	materialRendering->baseTexture = texture0 ? texture0->gpuResource.Get() : nullptr;
	materialRendering->vertexShader = vertexShader;
	materialRendering->fragmentShader = fragmentShader;
	materialRendering->normalMap = normalMap ? normalMap->gpuResource.Get() : nullptr;
	materialRendering->shadingModel = shadingModel;
	materialRendering->materialProperties = GetGraphicsInterface().CreateUniformBuffer(sizeof(MaterialProperties));
}
