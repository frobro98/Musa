// Copyright 2020, Nathan Blane

#pragma once

#include "Types/Intrinsics.hpp"
#include "Containers/DynamicArray.hpp"

DynamicArray<uint8> LoadFileToMemory(const tchar* filePath);