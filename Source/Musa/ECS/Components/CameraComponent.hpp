#pragma once

#include "Camera/Camera.h"
#include "Math/Rect.hpp"
#include "ECS/Component.hpp"

struct CameraComponent : public Musa::Component
{
	DECLARE_COMPONENT(CameraComponent);

	CameraComponent();

	Matrix projection;
	Matrix view;

	Vector right;
	Vector up = Vector(0, 1, 0);
	Vector forward;

	Vector position = Vector();
	Vector lookAt = Vector(0, 0, 1);
	IntRect viewport;

	Color clearColor = Color::Black();

	float nearPlane;
	float farPlane;
	float fov;
	float aspectRatio;
	Rect nearDimensions = {};
	Rect farDimensions = {};

	uint32 pad[1] = { 0 };
};
