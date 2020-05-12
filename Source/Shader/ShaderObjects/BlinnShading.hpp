// Copyright 2020, Nathan Blane

#pragma once

#include "Shader/ShaderObjects/ShaderObject.hpp"
#include "Shader/ShaderDefinition.hpp"

class BlinnVert : public ShaderObject
{
	DECLARE_SHADER();
public:
	BlinnVert(const ShaderCompiledOutput& compiledOutput);

	static ShaderObject* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new BlinnVert(compiledOutput);
	}

};

class BlinnFrag : public ShaderObject
{
	DECLARE_SHADER();
public:
	BlinnFrag(const ShaderCompiledOutput& compiledOutput);

	static ShaderObject* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new BlinnFrag(compiledOutput);
	}

};