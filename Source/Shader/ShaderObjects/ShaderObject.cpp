#include "ShaderObject.hpp"
#include "Graphics/Vulkan/VulkanShaderManager.h"

ShaderObject::ShaderObject(const ShaderCompiledOutput& compiledOutput)
	: byteCode(compiledOutput.compiledCode)
{
	nativeShader = GetShaderManager().CreateShader(compiledOutput);
}

uint32 GetHash(ShaderObject& so)
{
	return so.byteCodeHash;
}
