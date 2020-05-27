// Copyright 2020, Nathan Blane

#pragma once

#include "Graphics/GraphicsResourceDefinitions.hpp"
#include "Shader/ShaderObjects/ShaderObject.hpp"
#include "Shader/ShaderDefinition.hpp"
#include "Shader/ShaderAPI.hpp"

class SHADER_API NormalMappingVert : public ShaderObject<NativeVertexShader>
{
	DECLARE_SHADER();
public:
	NormalMappingVert(const ShaderCompiledOutput& compiledOutput);
	
	static ShaderObjectBase* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new NormalMappingVert(compiledOutput);
	}
};

class SHADER_API NormalMappingFrag : public ShaderObject<NativeFragmentShader>
{
	DECLARE_SHADER();
public:
	NormalMappingFrag(const ShaderCompiledOutput& compiledOutput);

	static ShaderObjectBase* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new NormalMappingFrag(compiledOutput);
	}
};
