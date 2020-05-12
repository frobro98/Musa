// Copyright 2020, Nathan Blane

#pragma once

#include "Shader/ShaderObjects/ShaderObject.hpp"
#include "Shader/ShaderDefinition.hpp"

class EnvironmentMappingVert : public ShaderObject
{
	DECLARE_SHADER();
public:
	EnvironmentMappingVert(const ShaderCompiledOutput& compiledOutput);

	static ShaderObject* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new EnvironmentMappingVert(compiledOutput);
	}
};

class EnvironmentMappingFrag : public ShaderObject
{
	DECLARE_SHADER();
public:
	EnvironmentMappingFrag(const ShaderCompiledOutput& compiledOutput);

	static ShaderObject* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new EnvironmentMappingFrag(compiledOutput);
	}
};

