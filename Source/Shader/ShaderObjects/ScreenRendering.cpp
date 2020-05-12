// Copyright 2020, Nathan Blane

#include "ScreenRendering.hpp"

DEFINE_VERTEX_SHADER(
	ScreenRenderVert,
	"Provided/ScreenRender.shad", "main",
	ScreenRenderVert::InitializeCompiledShader
);

DEFINE_FRAGMENT_SHADER(ScreenRenderFrag,
	"Provided/ScreenRender.shad", "main",
	ScreenRenderFrag::InitializeCompiledShader);

ScreenRenderVert::ScreenRenderVert(const ShaderCompiledOutput& compiledOutput)
	: ShaderObject(compiledOutput)
{
}

ScreenRenderFrag::ScreenRenderFrag(const ShaderCompiledOutput& compiledOutput)
	: ShaderObject(compiledOutput)
{
}