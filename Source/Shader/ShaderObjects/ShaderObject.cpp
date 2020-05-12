// Copyright 2020, Nathan Blane

#include "ShaderObject.hpp"
#include "Graphics/Vulkan/VulkanShaderManager.h"

ShaderObject::ShaderObject(const ShaderCompiledOutput& compiledOutput)
	: byteCode(compiledOutput.compiledCode),
	// TODO - Need to actually compute the hash for the data
	byteCodeHash(0)
{
	nativeShader = GetShaderManager().CreateShader(compiledOutput);
}

uint32 GetHash(ShaderObject& so)
{
	return so.byteCodeHash;
}
