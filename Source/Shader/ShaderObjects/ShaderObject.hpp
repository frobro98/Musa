// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "Containers/MemoryBuffer.hpp"
#include "Shader/ShaderStructure.hpp"
#include "Graphics/GraphicsInterface.hpp"
#include "Shader/ShaderDll.hpp"


class SHADER_API ShaderObjectBase
{
public:
	ShaderObjectBase(const ShaderCompiledOutput& compiledOutput);
	virtual ~ShaderObjectBase() = default;

	forceinline ShaderStage GetShaderStage() const { return stage; }

	friend uint32 GetHash(ShaderObjectBase& so)
	{
		return so.byteCodeHash;
	}

protected:
	MemoryBuffer shaderCode;
	uint32 byteCodeHash;
	ShaderStage stage;
};

// Corresponds to a shader the exists in the engine.
// Contains the graphics resource that corresponds to the compiled shader
template <class ShaderType>
class SHADER_TEMPLATE ShaderObject : public ShaderObjectBase
{
public:
	ShaderObject(const ShaderCompiledOutput& compiledOutput);

	forceinline ShaderStage GetShaderStage() const { return stage; }
	forceinline ShaderType& GetNativeShader()
	{
		if (!nativeShader.IsValid())
		{
			InitializeNativeShader();
		}
		return *nativeShader;
	}

	friend uint32 GetHash(ShaderObject& so)
	{
		return so.byteCodeHash;
	}

private:
	void InitializeNativeShader()
	{
		if constexpr (std::is_same_v<ShaderType, NativeVertexShader>)
		{
			nativeShader = GetGraphicsInterface().CreateVertexShader(shaderCode);
		}
		else if (std::is_same_v<ShaderType, NativeFragmentShader>)
		{
			nativeShader = GetGraphicsInterface().CreateFragmentShader(shaderCode);
		}
		else
		{
			Assert(false);
		}
	}

private:
	UniquePtr<ShaderType> nativeShader;
};

template <class ShaderType>
ShaderObject<ShaderType>::ShaderObject(const ShaderCompiledOutput& compiledOutput)
	: ShaderObjectBase(compiledOutput)
{
}
