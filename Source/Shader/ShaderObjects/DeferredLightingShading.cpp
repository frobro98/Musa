// Copyright 2020, Nathan Blane

#include "DeferredLightingShading.hpp"

DEFINE_VERTEX_SHADER(DeferredLightingVert,
	"Provided/DeferredLightingRender.shad", "main",
	InitializeCompiledShader);

DEFINE_FRAGMENT_SHADER(DeferredLightingFrag,
	"Provided/DeferredLightingRender.shad", "main",
	InitializeCompiledShader);

DeferredLightingVert::DeferredLightingVert(const ShaderCompiledOutput& compiledOutput)
	: ShaderObject(compiledOutput)
{
}

DeferredLightingFrag::DeferredLightingFrag(const ShaderCompiledOutput& compiledOutput)
	: ShaderObject(compiledOutput)
{
}