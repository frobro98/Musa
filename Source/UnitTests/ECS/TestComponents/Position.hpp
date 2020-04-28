#pragma once

#include "ECS/Component.hpp"
#include "Math/Vector4.hpp"

struct Position : Musa::Component
{
	Vector4 position;
};
