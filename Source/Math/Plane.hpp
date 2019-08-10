#pragma once

#include "Math/Vector4.h"
#include "Math/BoundsVolumes.hpp"

struct Plane
{
	Vector4 topLeft;
	Vector4 topRight;
	Vector4 bottomLeft;
	Vector4 bottomRight;
};

bool PlaneIntersectsSphere()