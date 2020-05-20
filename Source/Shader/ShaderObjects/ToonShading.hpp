// Copyright 2020, Nathan Blane

#pragma once

#include "Graphics/GraphicsResourceDefinitions.hpp"
#include "Shader/ShaderObjects/ShaderObject.hpp"
#include "Shader/ShaderDefinition.hpp"
#include "Shader/ShaderDll.hpp"

class SHADER_API ToonVert : public ShaderObject<NativeVertexShader>
{
private:
	static ShaderObjectBase* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new ToonVert(compiledOutput);
	}

	DECLARE_SHADER();
public:
	ToonVert(const ShaderCompiledOutput& compiledOutput);

};

class SHADER_API ToonFrag : public ShaderObject<NativeFragmentShader>
{
private:
	static ShaderObjectBase* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new ToonFrag(compiledOutput);
	}

	DECLARE_SHADER();
public:
	ToonFrag(const ShaderCompiledOutput& compiledOutput);
};
