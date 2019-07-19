
#include "NormalMapping.h"

DEFINE_VERTEX_SHADER(NormalMappingVert,
	"Provided/TransformationVert.shad", "main",
	InitializeCompiledShader);

DEFINE_FRAGMENT_SHADER(NormalMappingFrag,
	"Provided/LightingSetup.shad", "main",
	InitializeCompiledShader,
	"NORMAL_MAP", "1",
	"BLINN_MODEL", "1",
	"USE_GBUFFER", "1");

NormalMappingVert::NormalMappingVert(const ShaderCompiledOutput& compiledOutput)
	: ShaderObject(compiledOutput)
{
}

NormalMappingFrag::NormalMappingFrag(const ShaderCompiledOutput& compiledOutput)
	: ShaderObject(compiledOutput)
{
}
