#pragma once

#include "Types.h"
#include "Containers/Array.h"
#include "File/Path.hpp"

Array<uint8> LoadFileToMemory(const Path& filePath);