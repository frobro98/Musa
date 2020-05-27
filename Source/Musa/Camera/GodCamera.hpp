// Copyright 2020, Nathan Blane

#pragma once

#include "GameObject/GameObject.h"
#include "Math/Vector4.hpp"
#include "Input/Input.hpp"

class Camera;
struct FrameInputs;

class GodCamera : public GameObject
{
public:
	GodCamera(GameWorld& world, Camera& cam);

	virtual void Update(float tick) override;

	inline Camera& GetAssociatedCamera() const { return *camera; }

	void InputCallback(const FrameInputs& inputs);

private:
	void MoveCameraForward();
	void MoveCameraBackward();

	void MoveCameraLeft();
	void MoveCameraRight();

	void MoveCameraUp();
	void MoveCameraDown();

	void MoveCameraAlongAxis(const Vector4& axis, bool positive);

	void CameraLookAtAdjust(float changeX, float changeY);

private:
	Vector4 cameraUp;
	Vector4 cameraLookAt;
	Camera* camera;
	f32 moveSpeed = 0.f;
	f32 lookSpeed = 0.f;
};
