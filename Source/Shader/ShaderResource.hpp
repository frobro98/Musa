#pragma once

#include "Graphics/Vulkan/VulkanDefinitions.h"

#include "Types/Uncopyable.hpp"
#include "Containers/DynamicArray.hpp"
#include "Shader/ShaderStructure.hpp"

class VulkanDevice;

struct ShaderResource : private Uncopyable
{
	ShaderResource(const VulkanDevice& device, const VulkanByteCode& code, ShaderStage stage, const tchar* entryPoint, const ShaderConstantByBinding& bindings);
	~ShaderResource();

	VkShaderModule nativeShader = VK_NULL_HANDLE;
	const VulkanDevice& logicalDevice;
	const tchar* const shaderEntryPoint;
	DynamicArray<ShaderConstant> shaderUniforms;
	VkShaderStageFlags shaderStage;
};
