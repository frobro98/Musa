// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "ShaderStages.hpp"
#include "Containers/MemoryBuffer.hpp"
#include "Containers/Map.h"
#include "String/String.h"

// TODO - Move this into the Graphics dll
enum class ShaderResourceType : u16
{
	UniformBuffer,
	UniformDynamicBuffer,
	StorageBuffer,
	StorageDynamicBuffer,
	TextureSampler
};

enum class ShaderIntrinsics
{
	Integer,
	Float,
	Double,
	Boolean,
	Vector,
	Matrix,
	Struct
};

struct ShaderVariable
{
	String name;
	u32 location;
	ShaderIntrinsics variableType;
};

struct ShaderConstant
{
	String name;
	u32 binding;
	ShaderResourceType bindingType;
};

struct ShaderCompilerDefinitions
{
	// File Name

	// Preprocessing Definitions
	Map<String, String> definitions;
	// Stage
	ShaderStage::Type shaderStage;
};

struct PreprocessedShaderOutput
{
	String errors;
	String outputGlsl;
};

struct ShaderDebugInfo
{
	String glslProcessedCode;
	String spirvByteCode;
};
