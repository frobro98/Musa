// Copyright 2020, Nathan Blane

#pragma once

#include "Graphics/GraphicsResourceDefinitions.hpp"
#include "Shader/ShaderObjects/ShaderObject.hpp"
#include "Shader/ShaderDefinition.hpp"
#include "Shader/ShaderAPI.hpp"

class SHADER_API PhongVert : public ShaderObject<NativeVertexShader>
{
private:
	static ShaderObjectBase* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new PhongVert(compiledOutput);
	}

	DECLARE_SHADER();
public:
	PhongVert(const ShaderCompiledOutput& compiledOutput);

};

class SHADER_API PhongFrag : public ShaderObject<NativeFragmentShader>
{
private:
	static ShaderObjectBase* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new PhongFrag(compiledOutput);
	}

	DECLARE_SHADER();
public:
	PhongFrag(const ShaderCompiledOutput& compiledOutput);

};
