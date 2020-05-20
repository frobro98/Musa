// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Uncopyable.hpp"
#include "Containers/MemoryBuffer.hpp"
#include "Graphics/GraphicsResourceDefinitions.hpp"
#include "Vulkan/VulkanDefinitions.h"
#include "Vulkan/VulkanShaderHeader.hpp"

// TODO - Loading shaders will be more than just loading the file. It will be compilation at runtime, reflection over source, etc.
MemoryBuffer LoadSPVShader(const tchar* shaderFile);

class VulkanDevice;

class VulkanShader : Uncopyable
{
public:
	VulkanShader(const VulkanDevice& device, VulkanShaderHeader& shaderHeader, const DynamicArray<uint32>& shaderCode);
	~VulkanShader();

	forceinline VkShaderModule GetModule() const { return shaderModule; }
	forceinline VkShaderStageFlags GetStage() const { return shaderStage; }
	forceinline const VulkanShaderHeader& GetShaderHeader() const { return shaderHeader; }

private:
	VulkanShaderHeader shaderHeader;
	const VulkanDevice& logicalDevice;
	VkShaderModule shaderModule;
	VkShaderStageFlags shaderStage;
};

struct VulkanVertexShader : VulkanShader, NativeVertexShader
{
	VulkanVertexShader(const VulkanDevice& device, VulkanShaderHeader& shaderHeader, const DynamicArray<uint32>& shaderCode);
};

struct VulkanFragmentShader : VulkanShader, NativeFragmentShader
{
	VulkanFragmentShader(const VulkanDevice& device, VulkanShaderHeader& shaderHeader, const DynamicArray<uint32>& shaderCode);
};

struct VulkanGeometryShader : VulkanShader, NativeGeometryShader
{
	VulkanGeometryShader(const VulkanDevice& device, VulkanShaderHeader& shaderHeader, const DynamicArray<uint32>& shaderCode);
};

struct VulkanTessEvaluationShader : VulkanShader, NativeTessEvaluationShader
{
	VulkanTessEvaluationShader(const VulkanDevice& device, VulkanShaderHeader& shaderHeader, const DynamicArray<uint32>& shaderCode);
};

struct VulkanTessControlShader : VulkanShader, NativeTessControlShader
{
	VulkanTessControlShader(const VulkanDevice& device, VulkanShaderHeader& shaderHeader, const DynamicArray<uint32>& shaderCode);
};

struct VulkanComputeShader : VulkanShader, NativeComputeShader
{
	VulkanComputeShader(const VulkanDevice& device, VulkanShaderHeader& shaderHeader, const DynamicArray<uint32>& shaderCode);
};
