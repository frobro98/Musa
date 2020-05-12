// Copyright 2020, Nathan Blane

#include "SimplePrimitiveRendering.hpp"

DEFINE_VERTEX_SHADER(SimplePrimitiveVert,
	"Provided/SimplePrimitiveShader.shad", "main",
	InitializeCompiledShader);

DEFINE_FRAGMENT_SHADER(SimplePrimitiveFrag,
	"Provided/SimplePrimitiveShader.shad", "main",
	InitializeCompiledShader,
	"USE_GBUFFER", "1");

SimplePrimitiveVert::SimplePrimitiveVert(const ShaderCompiledOutput& compiledOutput)
	: ShaderObject(compiledOutput)
{
}

SimplePrimitiveFrag::SimplePrimitiveFrag(const ShaderCompiledOutput& compiledOutput)
	: ShaderObject(compiledOutput)
{
}