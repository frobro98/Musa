// Copyright 2020, Nathan Blane

#pragma once

#include "Graphics/GraphicsResourceDefinitions.hpp"
#include "Shader/ShaderObjects/ShaderObject.hpp"
#include "Shader/ShaderDefinition.hpp"
#include "Shader/ShaderDll.hpp"

class SHADER_API UnlitVert : public ShaderObject<NativeVertexShader>
{
private:
	static ShaderObjectBase* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new UnlitVert(compiledOutput);
	}

	DECLARE_SHADER();
public:
	UnlitVert(const ShaderCompiledOutput& compiledOutput);
};

class SHADER_API UnlitFrag : public ShaderObject<NativeFragmentShader>
{
private:
	static ShaderObjectBase* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new UnlitFrag(compiledOutput);
	}

	DECLARE_SHADER();
public:
	UnlitFrag(const ShaderCompiledOutput& compiledOutput);
};