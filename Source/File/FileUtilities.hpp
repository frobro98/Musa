#pragma once

#include "Types/Intrinsics.hpp"
#include "Containers/DynamicArray.hpp"
#include "File/Path.hpp"

DynamicArray<uint8> LoadFileToMemory(const Path& filePath);