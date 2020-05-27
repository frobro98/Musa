// Copyright 2020, Nathan Blane

#pragma once

#include "Graphics/GraphicsResourceDefinitions.hpp"
#include "Shader/ShaderObjects/ShaderObject.hpp"
#include "Shader/ShaderDefinition.hpp"
#include "Shader/ShaderAPI.hpp"

class SHADER_API DeferredLightingVert : public ShaderObject<NativeVertexShader>
{
	DECLARE_SHADER();
public:
	DeferredLightingVert(const ShaderCompiledOutput& compiledOutput);

	static ShaderObjectBase* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new DeferredLightingVert(compiledOutput);
	}

};

class SHADER_API DeferredLightingFrag : public ShaderObject<NativeFragmentShader>
{
	DECLARE_SHADER();
public:
	DeferredLightingFrag(const ShaderCompiledOutput& compiledOutput);

	static ShaderObjectBase* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new DeferredLightingFrag(compiledOutput);
	}

};
