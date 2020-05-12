// Copyright 2020, Nathan Blane

#pragma once

#include "Camera/Camera.h"
#include "Math/Rect.hpp"
#include "ECS/Component.hpp"

struct CameraComponent : public Musa::Component
{
	//DECLARE_COMPONENT(CameraComponent);

	CameraComponent();

	Matrix4 projection;
	Matrix4 view;

	Vector4 right;
	Vector4 up = Vector4(0, 1, 0);
	Vector4 forward;

	Vector4 position = Vector4();
	Vector4 lookAt = Vector4(0, 0, 1);
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
