
#include "VulkanShaderManager.h"
#include "VulkanShader.h"
#include "Shader/ShaderStructure.hpp"

// TODO - The shader resource that lives in the Shader.lib doesn't belong here..
#include "Shader/ShaderResource.hpp"


ShaderResource* VulkanShaderManager::CreateShader(const ShaderCompiledOutput& compiledOutput)
{
	ShaderResource* shader = new ShaderResource(*logicalDevice, compiledOutput.compiledCode, compiledOutput.stage, compiledOutput.shaderEntryPoint, compiledOutput.bindingToConstants);
	shaders.Add(shader);
	return shader;
}

VulkanShaderManager::~VulkanShaderManager()
{
	for (auto shader : shaders)
	{
		delete shader;
	}
}

VulkanShaderManager& GetShaderManager()
{
	static VulkanShaderManager vmInstance;
	return vmInstance;
}
