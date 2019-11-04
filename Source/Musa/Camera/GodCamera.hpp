#pragma once

#include "GameObject/GameObject.h"
#include "Math/Vector4.h"
#include "Input/Input.hpp"

class Camera;
struct FrameInputs;

class GodCamera : public GameObject
{
public:
	GodCamera(Camera& cam);

	virtual void Update(float tick) override;

	inline Camera& GetAssociatedCamera() const { return camera; }

	void InputCallback(const FrameInputs& inputs);

private:
	void SetupGodInputInput();

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
	Camera& camera;
	float32 moveSpeed = 0.f;
};
