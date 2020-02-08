
#include "DeferredLightingShading.hpp"

DEFINE_VERTEX_SHADER(DeferredLightingVert,
	"Provided/ScreenRender.shad", "main",
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