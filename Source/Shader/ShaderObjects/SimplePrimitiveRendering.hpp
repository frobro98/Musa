// Copyright 2020, Nathan Blane

#pragma once

#include "Shader/ShaderObjects/ShaderObject.hpp"
#include "Shader/ShaderDefinition.hpp"

class SimplePrimitiveVert : public ShaderObject
{
	DECLARE_SHADER();
public:
	SimplePrimitiveVert(const ShaderCompiledOutput& compiledOutput);

	static ShaderObject* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new SimplePrimitiveVert(compiledOutput);
	}

};

class SimplePrimitiveFrag : public ShaderObject
{
	DECLARE_SHADER();
public:
	SimplePrimitiveFrag(const ShaderCompiledOutput& compiledOutput);

	static ShaderObject* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new SimplePrimitiveFrag(compiledOutput);
	}

};
