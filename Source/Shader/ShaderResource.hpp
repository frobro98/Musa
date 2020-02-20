#pragma once

#include "Graphics/Vulkan/VulkanDefinitions.h"

#include "Containers/DynamicArray.hpp"
#include "Shader/ShaderStructure.hpp"

class VulkanDevice;

struct ShaderResource
{
	ShaderResource(const VulkanDevice& device, const VulkanByteCode& code, ShaderStage stage, const tchar* entryPoint, const ShaderConstantByBinding& bindings);
	~ShaderResource();

	ShaderResource& operator=(const ShaderResource&) = delete;

	VkShaderModule nativeShader = VK_NULL_HANDLE;
	const VulkanDevice& logicalDevice;
	const tchar* const shaderEntryPoint;
	DynamicArray<ShaderConstant> shaderUniforms;
	VkShaderStageFlags shaderStage;
	uint32 pad[1] = { 0 };
};
