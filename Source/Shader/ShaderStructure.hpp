// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "ShaderStages.hpp"
#include "Containers/MemoryBuffer.hpp"
#include "Containers/Map.h"
#include "String/String.h"

// TODO - Move this into the Graphics dll
enum class ShaderConstantType
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
	uint32 location;
	ShaderIntrinsics variableType;
};

struct ShaderConstant
{
	String name;
	uint32 binding;
	ShaderConstantType bindingType;
};

struct ShaderCompilerDefinitions
{
	// File Name

	// Preprocessing Definitions
	Map<String, String> definitions;
	// Stage
	ShaderStage shaderStage;
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

using ShaderInputByLocation = Map<uint32, ShaderVariable>;
using ShaderOutputByLocation = Map<uint32, ShaderVariable>;
using ShaderConstantByBinding = Map<uint32, ShaderConstant>;

struct ShaderCompiledOutput
{
	ShaderInputByLocation locationToInputs;
	ShaderOutputByLocation locationToOutputs;

	ShaderConstantByBinding bindingToConstants;
	MemoryBuffer shaderCode;
	const tchar* shaderEntryPoint;
	
	ShaderStage stage;
};

struct ShaderStructure
{
	ShaderCompiledOutput compiledOutput;
	ShaderDebugInfo readableShaderInfo;
};