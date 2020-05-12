// Copyright 2020, Nathan Blane

#include "ShaderCompiler.h"

#include "Containers/Map.h"
#include "String/String.h"

namespace
{
static Map<String, DynamicArray<uint32>> PathToSpvSource;
}

