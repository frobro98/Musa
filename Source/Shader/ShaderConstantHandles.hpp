// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "String/String.h"
#include "Debugging/Assertion.hpp"
#include "Shader/ShaderTables.hpp"

struct ShaderConstantNameMap;
struct NativeUniformBuffer;
struct NativeTexture;

// Index into the resource table "header" for where the resource actually lives
struct UniformBufferShaderConstant
{
	u16 tableIndex;
	u16 stage;
};

// Index into the resource table "header" for where the resource actually lives
struct TextureShaderConstant
{
	u16 tableIndex;
	ShaderStage stage;
};

u16 GetUniformBufferTableIndex(const ShaderConstantNameMap& resourceMap, const String& name);
u16 GetTextureTableIndex(const ShaderConstantNameMap& resourceMap, const String& name);


//////////////////////////////////////////////////////////////////////////
// Shader Constant Header Accessors
//////////////////////////////////////////////////////////////////////////

forceinline ShaderBindInfo GetUniformBufferConstantInfo(const ShaderConstantTable& table, UniformBufferShaderConstant resource)
{
	AssertStr(resource.tableIndex < table.textureResourceInfo.Size(),
		"UniformBuffer resource is outside the table's bounds!");
	return table.uniformBufferResourceInfo[resource.tableIndex];
}

forceinline ShaderBindInfo GetTextureConstantInfo(const ShaderConstantTable& table, TextureShaderConstant resource)
{
	AssertStr(resource.tableIndex < table.textureResourceInfo.Size(),
		"Texture resource is outside the table's bounds!");
	return table.textureResourceInfo[resource.tableIndex];
}

//////////////////////////////////////////////////////////////////////////
// Shader Resource Storage Accessors
//////////////////////////////////////////////////////////////////////////

forceinline NativeUniformBuffer*& GetUniformBufferResource(ShaderResourceTable& table, TextureShaderConstant resHandle)
{
	AssertStr(resHandle.tableIndex < table.uniformBufferResourceInfo.Size(),
		"Texture resource is outside the table's bounds!");
	return table.uniformBufferResourceInfo[resHandle.tableIndex];
}

forceinline NativeTexture*& GetTextureResource(ShaderResourceTable& table, TextureShaderConstant resHandle)
{
	AssertStr(resHandle.tableIndex < table.textureResourceInfo.Size(),
		"Texture resource is outside the table's bounds!");
	return table.textureResourceInfo[resHandle.tableIndex];
}
