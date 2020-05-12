// Copyright 2020, Nathan Blane

#pragma once

#include "Math/Vector4.hpp"
#include "Math/BoundsVolumes.hpp"

struct Plane
{
	Vector4 topLeft;
	Vector4 topRight;
	Vector4 bottomLeft;
	Vector4 bottomRight;
};

bool PlaneIntersectsSphere();