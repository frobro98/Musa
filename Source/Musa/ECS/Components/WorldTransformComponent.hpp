// Copyright 2020, Nathan Blane

#pragma once

#include "ECS/Component.hpp"
#include "Math/Matrix4.hpp"

struct WorldTransformComponent : Musa::Component
{
	Matrix4 transform;
};
