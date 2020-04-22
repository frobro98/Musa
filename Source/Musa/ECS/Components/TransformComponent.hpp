#pragma once

#include "ECS/Component.hpp"
#include "Math/Vector4.hpp"
#include "Math/Quat.hpp"
#include "Math/Matrix4.hpp"

struct TransformComponent : public Musa::Component
{
// 	Matrix4 worldTransform;
// 	Matrix4 localTransform;
	Vector4 position;
	Quat   rotation;
	Vector4 scale;

	bool transformDirty;
	bool wasUpdated;
};

