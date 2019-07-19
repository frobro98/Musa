#pragma once

#include "GameObject/GameObject.h"
#include "Math/Vector.h"

class Camera;

class GodCamera : public GameObject
{
public:
	GodCamera(Camera& cam);

	virtual void Update(float tick) override;

	inline Camera& GetAssociatedCamera() const { return camera; }

private:
	void SetupGodInputInput();

	void MoveCameraForward(int32 mod);
	void MoveCameraRight(int32 mod);
	void MoveCameraUp(int32 mod);

	void CameraLookAtAdjust(float changeX, float changeY);

private:
	Vector cameraUp;
	Vector cameraLookAt;
	Camera& camera;
};
