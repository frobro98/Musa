// Copyright 2020, Nathan Blane

#pragma once

#include "Graphics/GraphicsResourceDefinitions.hpp"
#include "Shader/ShaderObjects/ShaderObject.hpp"
#include "Shader/ShaderDefinition.hpp"
#include "Shader/ShaderAPI.hpp"

class SHADER_API BlinnVert : public ShaderObject<NativeVertexShader>
{
	DECLARE_SHADER();
public:
	BlinnVert(const ShaderCompiledOutput& compiledOutput);

	static ShaderObjectBase* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new BlinnVert(compiledOutput);
	}

};

class SHADER_API BlinnFrag : public ShaderObject<NativeFragmentShader>
{
	DECLARE_SHADER();
public:
	BlinnFrag(const ShaderCompiledOutput& compiledOutput);

	static ShaderObjectBase* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new BlinnFrag(compiledOutput);
	}

};