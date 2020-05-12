// Copyright 2020, Nathan Blane

#include "BlinnShading.hpp"

DEFINE_VERTEX_SHADER(BlinnVert,
	"Provided/TransformationVert.shad", "main",
	InitializeCompiledShader);

DEFINE_FRAGMENT_SHADER(BlinnFrag,
	"Provided/LightingSetup.shad", "main",
	InitializeCompiledShader,
	"BLINN_MODEL", "1",
	"USE_GBUFFER", "1");

BlinnVert::BlinnVert(const ShaderCompiledOutput& compiledOutput)
	: ShaderObject(compiledOutput)
{
}

BlinnFrag::BlinnFrag(const ShaderCompiledOutput& compiledOutput)
	: ShaderObject(compiledOutput)
{
}
