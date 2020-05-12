// Copyright 2020, Nathan Blane

#pragma once

#include "Shader/ShaderObjects/ShaderObject.hpp"
#include "Shader/ShaderDefinition.hpp"

class NormalMappingVert : public ShaderObject
{
	DECLARE_SHADER();
public:
	NormalMappingVert(const ShaderCompiledOutput& compiledOutput);
	
	static ShaderObject* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new NormalMappingVert(compiledOutput);
	}
};

class NormalMappingFrag : public ShaderObject
{
	DECLARE_SHADER();
public:
	NormalMappingFrag(const ShaderCompiledOutput& compiledOutput);

	static ShaderObject* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new NormalMappingFrag(compiledOutput);
	}
};
