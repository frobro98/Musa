// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "Shader/ShaderStructure.hpp"

struct ShaderResource;

// Corresponds to a shader the exists in the engine.
// Contains the graphics resource that corresponds to the compiled shader
class ShaderObject
{
public:
	ShaderObject(const ShaderCompiledOutput& compiledOutput);
	virtual ~ShaderObject() = default;

	inline ShaderStage GetShaderStage() const { return stage; }
	inline ShaderResource& GetNativeShader() const { return *nativeShader; }

	friend uint32 GetHash(ShaderObject& so);

private:
	VulkanByteCode byteCode;
	ShaderResource* nativeShader;
	uint32 byteCodeHash;
	ShaderStage stage;
};