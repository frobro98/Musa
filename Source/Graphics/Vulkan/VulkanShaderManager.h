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

	//VulkanShader* CreateShader(const tchar* shaderFile, ShaderStage stage);
	ShaderResource* CreateShader(const ShaderCompiledOutput& compiledOutput);
	//VulkanShader* CreateShader(const ShaderCompiledOutput& compiledOutput, ShaderStage stage);
	//VulkanShader* FindShader(const char* shaderName);

	// TODO - This doesn't make much sense, but because this currently is statically stored, this needs to be here...
	// TODO - Have the VulkanShaderManager not be vulkan specific and be stored somewhere easily accessible (not static!)
	void Deinitialize();

private:
	// This is because it needs a device. This needs to move or change because this isn't good
	friend class VulkanGraphicsInterface;

	DynamicArray<ShaderResource*> shaders;
	class VulkanDevice* logicalDevice = nullptr;
};

VulkanShaderManager& GetShaderManager();
