#pragma once

#include "Math/Vector.h"

struct BoxBounds
{
	Vector position;
	Vector dimensions;
};

struct SphereBounds
{
	Vector position;
	float radius;
};

// Combines both bounded box and sphere together
struct BoxSphereBounds
{
	Vector position;
	Vector dimensions;
	float radius;
};
