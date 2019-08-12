#pragma once

#include "Shader/ShaderObjects/ShaderObject.hpp"
#include "Shader/ShaderDefinition.hpp"

class UnlitVert : public ShaderObject
{
private:
	static ShaderObject* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new UnlitVert(compiledOutput);
	}

	DECLARE_SHADER();
public:
	UnlitVert(const ShaderCompiledOutput& compiledOutput);
};

class UnlitFrag : public ShaderObject
{
private:
	static ShaderObject* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new UnlitFrag(compiledOutput);
	}

	DECLARE_SHADER();
public:
	UnlitFrag(const ShaderCompiledOutput& compiledOutput);
};