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
	ShaderResource(const ShaderHeader& header, const MemoryBuffer& codeBuffer, ShaderStage::Type stage);

	forceinline NativeVertexShader* GetVertexShader()
	{
		Assert(stage == ShaderStage::Vertex);
		return vertexShader.Get();
	}

	forceinline NativeFragmentShader* GetFragmentShader()
	{
		Assert(stage == ShaderStage::Fragment);
		return fragmentShader.Get();
	}

	forceinline ShaderID GetShaderID() const { return header.id; }
	forceinline ShaderStage::Type GetShaderStage() const { return stage; }
	forceinline const ShaderConstantTable& GetResourceTable() const { return header.resourceTable; }
	forceinline const ShaderConstantNameMap& GetNameMap() const { return header.resourceNames; }

private:
	MemoryBuffer code;
	ShaderHeader header;
	UniquePtr<NativeVertexShader> vertexShader = nullptr;
	UniquePtr<NativeFragmentShader> fragmentShader = nullptr;
	ShaderStage::Type stage;
};