#pragma once

#include "Types/Intrinsics.hpp"

struct ShaderCompilerDefinitions;
struct ShaderStructure;
struct PreprocessedShaderOutput;

bool Preprocess(const tchar* pathToShader, const ShaderCompilerDefinitions& inputs, PreprocessedShaderOutput& output);
bool Compile(const tchar* pathToShader, const char* entryPoint, const ShaderCompilerDefinitions& inputs, ShaderStructure& outputs);