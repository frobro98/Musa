#pragma once

#include "CameraObject.h"

class Camera;

class HorizontalOrbitCamera : public CameraObject
{
public:
	HorizontalOrbitCamera(const char* camName);
	HorizontalOrbitCamera(int32 width, int32 height);
	~HorizontalOrbitCamera();

	void SetRotateSpeed(float rotSpeed);

	virtual void Update(float tick) override;

private:
	float32 rotationSpeed;
};

