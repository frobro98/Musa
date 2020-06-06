// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "Shader/ShaderAPI.hpp"

struct ShaderCompilerDefinitions;
struct ShaderStructure;
struct PreprocessedShaderOutput;

void SHADER_API InitializeCompiler();
void SHADER_API DeinitializeCompiler();
bool SHADER_API Preprocess(const tchar* pathToShader, const ShaderCompilerDefinitions& inputs, PreprocessedShaderOutput& output);
bool SHADER_API Compile(const tchar* pathToShader, const char* entryPoint, const ShaderCompilerDefinitions& inputs, ShaderStructure& outputs);