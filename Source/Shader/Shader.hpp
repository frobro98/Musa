#pragma once

#include "EngineCore/Types.h"
#include "Containers/Array.h"
#include "Shader/ShaderStages.hpp"

class Material;

// Represents a shader and parameters of the shader
class Shader
{
public:
	Shader(std::function<bool()> isSupportedShaderFunc, const tchar* fileName, ShaderStage stage);


private:
	ShaderStage stage;
};