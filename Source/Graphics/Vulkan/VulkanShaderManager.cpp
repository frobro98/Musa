
#include "VulkanShaderManager.h"
#include "VulkanShader.h"
#include "Shader/ShaderStructure.hpp"
#include "Shader/ShaderObjects/ShaderResource.hpp"


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
