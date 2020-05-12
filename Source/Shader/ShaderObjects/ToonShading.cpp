// Copyright 2020, Nathan Blane

#include "ToonShading.hpp"

DEFINE_VERTEX_SHADER(ToonVert,
	"Provided/TransformationVert.shad", "main",
	InitializeCompiledShader);

DEFINE_FRAGMENT_SHADER(ToonFrag,
	"Provided/LightingSetup.shad", "main",
	InitializeCompiledShader,
	"TOON_MODEL", "1",
	"USE_GBUFFER", "1");

ToonVert::ToonVert(const ShaderCompiledOutput& compiledOutput)
	: ShaderObject(compiledOutput)
{
}

ToonFrag::ToonFrag(const ShaderCompiledOutput& compiledOutput)
	: ShaderObject(compiledOutput)
{
}
