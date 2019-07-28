#pragma once

#include "Graphics.h"
#include "String/String.h"
#include "Containers/DynamicArray.hpp"
#include "Shader/ShaderStages.hpp"

struct ShaderResource;
class VulkanShader;
struct ShaderCompiledOutput;

class VulkanShaderManager
{
public:
	static constexpr const char* DefaultShader = "_default-shader";

	VulkanShaderManager() = default;
	~VulkanShaderManager();

	//VulkanShader* CreateShader(const tchar* shaderFile, ShaderStage stage);
	ShaderResource* CreateShader(const ShaderCompiledOutput& compiledOutput);
	//VulkanShader* CreateShader(const ShaderCompiledOutput& compiledOutput, ShaderStage stage);
	//VulkanShader* FindShader(const char* shaderName);


private:
	// This is because it needs a device. This needs to move or change because this isn't good
	friend class VulkanGraphicsInterface;

	DynamicArray<ShaderResource*> shaders;
	class VulkanDevice* logicalDevice = nullptr;
};

VulkanShaderManager& GetShaderManager();
