// Copyright 2020, Nathan Blane

#include "File/FileCore/File.h"

#include "String/String.h"
#include "DirectoryLocations.h"

#include "VulkanShaders.h"
#include "VulkanDevice.h"
#include "VulkanDescriptorSet.h"

// WALL_WRN_PUSH
// #include <shaderc/shaderc.hpp>
// WALL_WRN_POP

MemoryBuffer LoadSPVShader(const tchar* shaderFile)
{
	String filePath(EngineShaderSrcPath());
	filePath += shaderFile;
	File::Handle fHandle;
	File::Result result  = File::Open(fHandle, *filePath, File::Mode::READ);
	// TODO - This probably shouldn't be an assert. This should look in other places and then assert if it can't find files in the other places as well
	Assert(result == File::Result::SUCCESS);

	u32 fileSize;
	File::Seek(fHandle, File::Location::END, 0);
	File::Tell(fHandle, fileSize);
	File::Seek(fHandle, File::Location::BEGIN, 0);

	MemoryBuffer fileData(fileSize);
	result = File::Read(fHandle, fileData.GetData(), fileSize);
	Assert(result == File::Result::SUCCESS);

	File::Close(fHandle);

	return fileData;
}

//////////////////////////////////////////////////////////////////////////
// Vulkan Shader Base
//////////////////////////////////////////////////////////////////////////
VulkanShader::VulkanShader(const VulkanDevice& device, VulkanShaderHeader& header, const DynamicArray<u32>& shaderCode)
	: shaderHeader(header),
	logicalDevice(device),
	shaderStage(header.shaderStage)
{
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = shaderCode.SizeInBytes();
	createInfo.pCode = shaderCode.GetData();
	NOT_USED VkResult result = vkCreateShaderModule(logicalDevice.GetNativeHandle(), &createInfo, nullptr, &shaderModule);
	CHECK_VK(result);
}

VulkanShader::~VulkanShader()
{
	Assert(shaderModule != VK_NULL_HANDLE);
	vkDestroyShaderModule(logicalDevice.GetNativeHandle(), shaderModule, nullptr);
}

//////////////////////////////////////////////////////////////////////////
// Vulkan Vertex Shader
//////////////////////////////////////////////////////////////////////////
VulkanVertexShader::VulkanVertexShader(const VulkanDevice& device, VulkanShaderHeader& shaderHeader, const DynamicArray<u32>& shaderCode)
	: VulkanShader(device, shaderHeader, shaderCode)
{
	Assert(shaderHeader.shaderStage == VK_SHADER_STAGE_VERTEX_BIT);
}

//////////////////////////////////////////////////////////////////////////
// Vulkan Fragment Shader
//////////////////////////////////////////////////////////////////////////
VulkanFragmentShader::VulkanFragmentShader(const VulkanDevice& device, VulkanShaderHeader& shaderHeader, const DynamicArray<u32>& shaderCode)
	: VulkanShader(device, shaderHeader, shaderCode)
{
	Assert(shaderHeader.shaderStage == VK_SHADER_STAGE_FRAGMENT_BIT);
}

//////////////////////////////////////////////////////////////////////////
// Vulkan Geometry Shader
//////////////////////////////////////////////////////////////////////////
VulkanGeometryShader::VulkanGeometryShader(const VulkanDevice& device, VulkanShaderHeader& shaderHeader, const DynamicArray<u32>& shaderCode)
	: VulkanShader(device, shaderHeader, shaderCode)
{
	Assert(shaderHeader.shaderStage == VK_SHADER_STAGE_GEOMETRY_BIT);
}

//////////////////////////////////////////////////////////////////////////
// Vulkan Tessellation Evaluation Shader
//////////////////////////////////////////////////////////////////////////
VulkanTessEvaluationShader::VulkanTessEvaluationShader(const VulkanDevice& device, VulkanShaderHeader& shaderHeader, const DynamicArray<u32>& shaderCode)
	: VulkanShader(device, shaderHeader, shaderCode)
{
	Assert(shaderHeader.shaderStage == VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT);
}

//////////////////////////////////////////////////////////////////////////
// Vulkan Tessellation Control Shader
//////////////////////////////////////////////////////////////////////////
VulkanTessControlShader::VulkanTessControlShader(const VulkanDevice& device, VulkanShaderHeader& shaderHeader, const DynamicArray<u32>& shaderCode)
	: VulkanShader(device, shaderHeader, shaderCode)
{
	Assert(shaderHeader.shaderStage == VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT);
}

//////////////////////////////////////////////////////////////////////////
// Vulkan Compute Shader
//////////////////////////////////////////////////////////////////////////
VulkanComputeShader::VulkanComputeShader(const VulkanDevice& device, VulkanShaderHeader& shaderHeader, const DynamicArray<u32>& shaderCode)
	: VulkanShader(device, shaderHeader, shaderCode)
{
	Assert(shaderHeader.shaderStage == VK_SHADER_STAGE_COMPUTE_BIT);
}
