#pragma once

#include "Shader/ShaderObjects/ShaderObject.hpp"
#include "Shader/ShaderDefinition.hpp"

class ScreenRenderVert : public ShaderObject
{
private:
	static ShaderObject* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new ScreenRenderVert(compiledOutput);
	}

	DECLARE_SHADER();
public:
	ScreenRenderVert(const ShaderCompiledOutput& compiledOutput);

};

class ScreenRenderFrag : public ShaderObject
{
private:
	static ShaderObject* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new ScreenRenderFrag(compiledOutput);
	}

	DECLARE_SHADER();
public:
	ScreenRenderFrag(const ShaderCompiledOutput& compiledOutput);

};
