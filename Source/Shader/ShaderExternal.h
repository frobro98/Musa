#pragma once

#include "PlatformDefinitions.h"

// #if Vulkan
WALL_WRN_PUSH
#include "glslang/Public/ShaderLang.h"
#include "spirv/GlslangToSpv.h"
#include "spirv/disassemble.h"
#include "ThirdParty/spirv_reflect.h"
WALL_WRN_POP
//#endif