// Copyright 2020, Nathan Blane

#pragma once

#include "Shader/ShaderObjects/ShaderObject.hpp"
#include "Shader/ShaderDefinition.hpp"

class ToonVert : public ShaderObject
{
private:
	static ShaderObject* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new ToonVert(compiledOutput);
	}

	DECLARE_SHADER();
public:
	ToonVert(const ShaderCompiledOutput& compiledOutput);

};

class ToonFrag : public ShaderObject
{
private:
	static ShaderObject* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new ToonFrag(compiledOutput);
	}

	DECLARE_SHADER();
public:
	ToonFrag(const ShaderCompiledOutput& compiledOutput);
};
