// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "Containers/MemoryBuffer.hpp"
#include "File/FileAPI.hpp"

FILE_API MemoryBuffer LoadFileToMemory(const tchar* filePath);
