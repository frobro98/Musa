
#include "ShaderCompiler.h"

#include "Containers/Map.h"
#include "String/String.h"

namespace
{
static Map<String, Array<uint32>> PathToSpvSource;
}

