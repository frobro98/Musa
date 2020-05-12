// Copyright 2020, Nathan Blane

#pragma once

#include "Shader/ShaderObjects/ShaderObject.hpp"
#include "Shader/ShaderDefinition.hpp"

class DeferredLightingVert : public ShaderObject
{
	DECLARE_SHADER();
public:
	DeferredLightingVert(const ShaderCompiledOutput& compiledOutput);

	static ShaderObject* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new DeferredLightingVert(compiledOutput);
	}

};

class DeferredLightingFrag : public ShaderObject
{
	DECLARE_SHADER();
public:
	DeferredLightingFrag(const ShaderCompiledOutput& compiledOutput);

	static ShaderObject* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new DeferredLightingFrag(compiledOutput);
	}

};
