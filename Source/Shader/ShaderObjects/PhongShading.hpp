// Copyright 2020, Nathan Blane

#pragma once

#include "Shader/ShaderObjects/ShaderObject.hpp"
#include "Shader/ShaderDefinition.hpp"

class PhongVert : public ShaderObject
{
private:
	static ShaderObject* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new PhongVert(compiledOutput);
	}

	DECLARE_SHADER();
public:
	PhongVert(const ShaderCompiledOutput& compiledOutput);

};

class PhongFrag : public ShaderObject
{
private:
	static ShaderObject* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new PhongFrag(compiledOutput);
	}

	DECLARE_SHADER();
public:
	PhongFrag(const ShaderCompiledOutput& compiledOutput);

};
