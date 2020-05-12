// Copyright 2020, Nathan Blane

#pragma once

#include "EngineCore/Types/Intrinsics.hpp"

struct Extents2D
{
	float32 width;
	float32 height;

	friend bool operator==(const Extents2D& lhs, const Extents2D& rhs)
	{
		return lhs.width == rhs.width && lhs.height == rhs.height;
	}

	friend bool operator!=(const Extents2D& lhs, const Extents2D& rhs)
	{
		return lhs.width != rhs.width && lhs.height != rhs.height;
	}
};

struct Extents3D
{
	float32 width;
	float32 height;
	float32 depth;
};

