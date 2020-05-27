// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "Containers/DynamicArray.hpp"
#include "Graphics/ResourceBlob.hpp"

struct MipmapLevel
{
	ResourceBlob mipData;
	u32 width = 0;
	u32 height = 0;
};