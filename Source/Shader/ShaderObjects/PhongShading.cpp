// Copyright 2020, Nathan Blane

#include "PhongShading.hpp"

DEFINE_VERTEX_SHADER(PhongVert,
	"Provided/TransformationVert.shad", "main",
	InitializeCompiledShader);

DEFINE_FRAGMENT_SHADER(PhongFrag,
	"Provided/LightingSetup.shad", "main",
	InitializeCompiledShader,
	"PHONG_MODEL", "1",
	"USE_GBUFFER", "1");

PhongVert::PhongVert(const ShaderCompiledOutput& compiledOutput)
	: ShaderObject(compiledOutput)
{
}

PhongFrag::PhongFrag(const ShaderCompiledOutput& compiledOutput)
	: ShaderObject(compiledOutput)
{
}
