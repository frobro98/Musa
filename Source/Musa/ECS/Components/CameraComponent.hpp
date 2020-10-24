// Copyright 2020, Nathan Blane

#pragma once

#include "Camera/Camera.h"
#include "Math/Rect.hpp"
#include "Math/Vector4.hpp"
#include "ECS/Component.hpp"

struct CameraComponent : public Musa::Component
{
	Vector4 right;
	Vector4 up = Vector4(0, 1, 0);
	Vector4 forward;

	Vector4 position = Vector4();
	Vector4 lookAt = Vector4(0, 0, 1);
	Recti viewport;

	Color clearColor = Color::Black();

	float nearPlane;
	float farPlane;
	float fov;
	float aspectRatio;
	Rectf nearDimensions = {};
	Rectf farDimensions = {};
};
