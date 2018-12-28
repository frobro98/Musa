#include "Material.h"
#include "Graphics.h"
#include "Texture2D/TextureManager.h"
#include "Texture2D/Texture.h"
#include "Camera/CameraManager.h"
#include "Camera/Camera.h"

// TODO - Remove all vulkan from Material.h/.cpp
#include "Renderer/PipelineManager.h"

Material::Material()
{
	pipeline = PipelineManager::FindPipeline("defaultPipeline");
	texture0 = TextureManager::FindTexture(TextureManager::DefaultTexture);
	diffuseColor = Color::Magenta();
}

Material::Material(const char * pipelineName, const char * textureName, const Color & color)
{
	assert(pipelineName);
	pipeline = PipelineManager::FindPipeline(pipelineName);
	if (textureName != nullptr)
	{
		texture0 = TextureManager::FindTexture(textureName);
	}
	else
	{
		texture0 = TextureManager::FindTexture(TextureManager::DefaultTexture);
	}
	diffuseColor = color;
}

void Material::EnableWireframe()
{
	// TODO - set wire frame as a flag or some kind of check
}

void Material::DisableWireframe()
{
	// TODO - set wire frame as a flag or some kind of check
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Material::SetPipeline(VulkanPipeline* pipeline_)
{
	pipeline = pipeline_;
}

void Material::SetTexture0(Texture * tex0)
{
	texture0 = tex0;
}

void Material::SetTexture1(Texture * tex1)
{
	texture1 = tex1;
}

void Material::SetTexture2(Texture * tex2)
{
	texture2 = tex2;
}

void Material::SetColor(const Color& color)
{
	diffuseColor = color;
}
