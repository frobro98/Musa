
#include "VulkanShaderManager.h"
#include "VulkanShader.h"
#include "Shader/ShaderStructure.hpp"
#include "Shader/ShaderObjects/ShaderResource.hpp"

// VulkanShader* VulkanShaderManager::CreateShader(const tchar* shaderFile, ShaderStage stage)
// {
// 	DynamicArray<uint8> code = LoadSPVShader(shaderFile);
// 
// 	VulkanShader* shader = new VulkanShader(logicalDevice);
// 	shader->Compile(code, stage);
// 	return shader;
// }

ShaderResource* VulkanShaderManager::CreateShader(const ShaderCompiledOutput& compiledOutput)
{
	ShaderResource* shader = new ShaderResource(*logicalDevice, compiledOutput.compiledCode, compiledOutput.stage, compiledOutput.shaderEntryPoint, compiledOutput.bindingToConstants);
	return shader;
}

VulkanShaderManager::~VulkanShaderManager()
{
}

VulkanShaderManager& GetShaderManager()
{
	static VulkanShaderManager vmInstance;
	return vmInstance;
}
