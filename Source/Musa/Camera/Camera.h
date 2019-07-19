#pragma once

#include "Math/MathEngine.h"
#include "Math/Rect.hpp"
#include "Texture/Color.hpp"
#include "EngineCore/Extents.hpp"

class Camera final
{
public:
	Camera() = default;

	void SetPerspective(float fovDeg, float aspectRatio, float nearPlane, float farPlane);
	void SetViewport(const IntRect& vp);
	void SetOrientationAndPosition(const Vector& lookAt, const Vector& pos, const Vector& up);

	void Update();

	Vector GetPosition() const;
	Vector GetForward() const;
	Vector GetUp() const;
	Vector GetRight() const;
	Vector GetLookAt() const;

	// TODO - This most definitely should be somewhere else, like a Window class
	int GetScreenWidth() const;
	int GetScreenHeight() const;

	float GetFov() const;
	void SetFov(float fovDeg);

	float GetAspectRatio() const;
	void SetAspectRatio(float aspect);

	float GetNearPlane() const;
	void SetNearPlane(float near);

	float GetFarPlane() const;
	void SetFarPlane(float far);

	IntRect GetCameraViewport() const;

private:
	Matrix projection;
	Matrix view;

	// view matrix
	Vector viewRight;
	Vector viewUp;
	Vector viewForward;
	
	Vector position;
	Vector lookAt;

	IntRect viewport;

	Color clearColor{ Color::Black() };

	float nearPlane = 0;
	float farPlane = 1;
	float fov = 90;
	float aspectRatio = 0;

};