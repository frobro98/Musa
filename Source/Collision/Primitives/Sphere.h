#pragma once

#include "Math/Vector.h"

struct Sphere
{
	Vector cntr;
	float rad = 0;
	float pad[3] = { 0, 0, 0 };  // love and hate this new compiler
};

