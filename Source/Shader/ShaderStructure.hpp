#pragma once

#include "Types.h"
#include "ShaderStages.hpp"
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "String/String.h"

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
	Matrix
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

struct ShaderSettings
{
	// Platform
	
	// Stage
	ShaderStage shaderStage;
	// File Name

	// Preprocessing Definitions
	Map<String, String> definitions;
	// Code
	String shaderCode;
	// Name
	String shaderName;
};

struct ShaderIntermediate
{
	ShaderIntermediate(const ShaderSettings& settings)
		: stage(settings.shaderStage),
		definitions(settings.definitions)
	{
	}

	ShaderIntermediate& operator=(const ShaderIntermediate&) = delete;

	ShaderStage stage;
	const Map<String, String>& definitions;
	String preprocessedCode;
};

struct ShaderStructure
{
	Map<uint32, ShaderVariable> locationToInputs;
	Map<uint32, ShaderVariable> locationToOutputs;

	Map<uint32, ShaderConstant> bindingToConstants;
	Array<uint8> compiledCode;
};