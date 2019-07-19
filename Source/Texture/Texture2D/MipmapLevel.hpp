#pragma once

#include "Types/Intrinsics.hpp"
#include "Containers/DynamicArray.hpp"
#include "Graphics/ResourceBlob.hpp"

struct MipmapLevel
{
	ResourceBlob mipData;
	uint32 width = 0;
	uint32 height = 0;
};