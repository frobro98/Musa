#pragma once

#include "Math/Vector.h"
#include "Math/BoundsVolumes.hpp"

struct Plane
{
	Vector topLeft;
	Vector topRight;
	Vector bottomLeft;
	Vector bottomRight;
};

bool PlaneIntersectsSphere()