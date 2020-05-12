// Copyright 2020, Nathan Blane

#pragma once

#include "EngineCore/Types.h"
#include "Shader/ShaderStages.hpp"

class Shader
{
public:
	Shader();

	inline ShaderStage GetShaderStage() const { return stage; }

private:
	ShaderStage stage;
};