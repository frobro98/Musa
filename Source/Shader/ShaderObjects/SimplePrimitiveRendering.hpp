// Copyright 2020, Nathan Blane

#pragma once

#include "Graphics/GraphicsResourceDefinitions.hpp"
#include "Shader/ShaderObjects/ShaderObject.hpp"
#include "Shader/ShaderDefinition.hpp"
#include "Shader/ShaderDll.hpp"

class SHADER_API SimplePrimitiveVert : public ShaderObject<NativeVertexShader>
{
	DECLARE_SHADER();
public:
	SimplePrimitiveVert(const ShaderCompiledOutput& compiledOutput);

	static ShaderObjectBase* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new SimplePrimitiveVert(compiledOutput);
	}

};

class SHADER_API SimplePrimitiveFrag : public ShaderObject<NativeFragmentShader>
{
	DECLARE_SHADER();
public:
	SimplePrimitiveFrag(const ShaderCompiledOutput& compiledOutput);

	static ShaderObjectBase* InitializeCompiledShader(const ShaderCompiledOutput& compiledOutput)
	{
		return new SimplePrimitiveFrag(compiledOutput);
	}

};
