// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/UniquePtr.hpp"
#include "Debugging/Assertion.hpp"
#include "Shader/ShaderStages.hpp"
#include "Shader/ShaderHeader.hpp"
#include "Containers/MemoryBuffer.hpp"

struct NativeVertexShader;
struct NativeFragmentShader;

class ShaderResource
{
public:
	ShaderResource(const ShaderHeader& header, const MemoryBuffer& codeBuffer);

	forceinline NativeVertexShader* GetVertexShader()
	{
		Assert(header.stage == ShaderStage::Vertex);
		return vertexShader.Get();
	}

	forceinline NativeFragmentShader* GetFragmentShader()
	{
		Assert(header.stage == ShaderStage::Fragment);
		return fragmentShader.Get();
	}

	forceinline ShaderID GetShaderID() const { return header.id; }
	forceinline ShaderStage::Type GetShaderStage() const { return header.stage; }
	forceinline const ShaderConstantTable& GetResourceTable() const { return header.resourceTable; }
	forceinline const ShaderConstantNameMap& GetNameMap() const { return header.resourceNames; }

private:
	MemoryBuffer code;
	ShaderHeader header;
	UniquePtr<NativeVertexShader> vertexShader = nullptr;
	UniquePtr<NativeFragmentShader> fragmentShader = nullptr;
};