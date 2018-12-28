#pragma once

#include "Types.h"

struct ShaderSettings;
struct ShaderStructure;


bool Compile(const ShaderSettings& inputs, ShaderStructure& outputs);