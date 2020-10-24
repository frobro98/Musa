// Copyright 2020, Nathan Blane

#include "ShaderObject.hpp"
#include "Graphics/Vulkan/VulkanShaderManager.h"

ShaderObjectBase::ShaderObjectBase(const ShaderCompiledOutput& compiledOutput)
	: header(compiledOutput.header),
	shaderCode(compiledOutput.shaderCode),
	stage(compiledOutput.stage),
	// TODO - Need to actually compute the hash for the data
	byteCodeHash()
{
}
