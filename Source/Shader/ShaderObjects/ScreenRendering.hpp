// Copyright 2020, Nathan Blane

#pragma once

#include "Graphics/GraphicsResourceDefinitions.hpp"
#include "Shader/ShaderObjects/ShaderObject.hpp"
#include "Shader/ShaderDefinition.hpp"
#include "Shader/ShaderAPI.hpp"

class SHADER_API ScreenRenderVert : public ShaderObject<NativeVertexShader>
{
private:
	static ShaderObjectBase* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new ScreenRenderVert(compiledOutput);
	}

	DECLARE_SHADER();
public:
	ScreenRenderVert(const ShaderCompiledOutput& compiledOutput);

};

class SHADER_API ScreenRenderFrag : public ShaderObject<NativeFragmentShader>
{
private:
	static ShaderObjectBase* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new ScreenRenderFrag(compiledOutput);
	}

	DECLARE_SHADER();
public:
	ScreenRenderFrag(const ShaderCompiledOutput& compiledOutput);

};
