// Copyright 2020, Nathan Blane

#include "UnlitShading.hpp"

DEFINE_VERTEX_SHADER(UnlitVert,
	"Provided/TransformationVert.shad", "main",
	InitializeCompiledShader);

DEFINE_FRAGMENT_SHADER(UnlitFrag,
	"Provided/LightingSetup.shad", "main",
	InitializeCompiledShader,
	"UNLIT_MODEL", "1",
	"USE_GBUFFER", "1");

UnlitVert::UnlitVert(const ShaderCompiledOutput& compiledOutput)
	: ShaderObject(compiledOutput)
{
}

UnlitFrag::UnlitFrag(const ShaderCompiledOutput& compiledOutput)
	: ShaderObject(compiledOutput)
{
}
