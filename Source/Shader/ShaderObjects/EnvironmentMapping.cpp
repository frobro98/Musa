// Copyright 2020, Nathan Blane

#include "EnvironmentMapping.hpp"

// TODO - The fragment shader causes a compiler error because a lighting model isn't at all defined...
// DEFINE_VERTEX_SHADER(EnvironmentMappingVert,
// 	"Provided/TransformationVert.shad", "main",
// 	InitializeCompiledShader);
// 
// DEFINE_FRAGMENT_SHADER(EnvironmentMappingFrag,
// 	"Provided/LightingSetup.shad", "main",
// 	InitializeCompiledShader,
// 	"CUBE_MAP", "1",
// 	"USE_GBUFFER", "1");
// 
// 
// EnvironmentMappingVert::EnvironmentMappingVert(const ShaderCompiledOutput& compiledOutput)
// 	: ShaderObject(compiledOutput)
// {
// }
// 
// EnvironmentMappingFrag::EnvironmentMappingFrag(const ShaderCompiledOutput& compiledOutput)
// 	: ShaderObject(compiledOutput)
// {
// }
