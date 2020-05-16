// Copyright 2020, Nathan Blane

#pragma once

#include "VulkanDefinitions.h"

#include "Containers/MemoryBuffer.hpp"
#include "Shader/ShaderStages.hpp"

// TODO - Loading shaders will be more than just loading the file. It will be compilation at runtime, reflection over source, etc.
MemoryBuffer LoadSPVShader(const tchar* shaderFile);

class VulkanDevice;

class VulkanShader
{
public:
	VulkanShader(VulkanDevice* device);
	~VulkanShader();

	void Compile(const MemoryBuffer& source, ShaderStage stage);

	inline const uint32* GetSource() const { return shaderSource; }
	inline VkShaderModule GetModule() const { return shaderModule; }
	inline ShaderStage GetShaderStage() const { return shaderStage; }
private:
	VkShaderModule shaderModule = VK_NULL_HANDLE;
	VulkanDevice* logicalDevice;
	uint32* shaderSource = nullptr;
	size_t sourceSize = 0;
	ShaderStage shaderStage;
	uint32 pad[1] = { 0 };
};
