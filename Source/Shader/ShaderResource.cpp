#include "ShaderResource.hpp"
#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanUtilities.h"

ShaderResource::ShaderResource(const VulkanDevice& device, const VulkanByteCode& code, ShaderStage stage, const tchar* entryPoint, const ShaderConstantByBinding& bindings)
	: logicalDevice(device),
	shaderEntryPoint(entryPoint),
	shaderStage(MusaStageToVkStage(stage))
{
	VkShaderModuleCreateInfo shaderCreateInfo = {};
	shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderCreateInfo.codeSize = code.SizeInBytes();
	shaderCreateInfo.pCode = code.GetData();
	CHECK_VK(vkCreateShaderModule(logicalDevice.GetNativeHandle(), &shaderCreateInfo, nullptr, &nativeShader));

	for (const auto& constantIndexPair : bindings)
	{
		shaderUniforms.Add(constantIndexPair.second);
	}
}

ShaderResource::~ShaderResource()
{
	Assert(nativeShader != VK_NULL_HANDLE);
	vkDestroyShaderModule(logicalDevice.GetNativeHandle(), nativeShader, nullptr);
}
