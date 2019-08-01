
#include "Camera.h"
#include "GameObject/GameObject.h"

#undef near
#undef far

void Camera::SetPerspective(float fovDeg, float aspect, float near, float far)
{
	aspectRatio = aspect;
	fov = fovDeg;
	nearPlane = near;
	farPlane = far;
}

void Camera::SetViewport(const IntRect& vp)
{
	viewport = vp;
}

void Camera::SetOrientationAndPosition(const Vector& inLookAt, const Vector& pos, const Vector& up)
{
	lookAt = inLookAt;
	position = pos;

	viewForward = position - lookAt;
	viewForward.Normalize();
	
	viewRight = up.Cross(viewForward);
	viewRight.Normalize();

	viewUp = viewForward.Cross(viewRight);
	viewUp.Normalize();
}

// void Camera::CalculatePlaneDimentions()
// {
// 	nearHeight = 2.f * Math::Tan(Math::DegreesToRadians(fov) * .5f) * nearPlane;
// 	nearWidth = nearHeight * aspectRatio;
// 	farHeight = 2.f * Math::Tan(Math::DegreesToRadians(fov) * .5f) * farPlane;
// 	farWidth = farHeight * aspectRatio;
// }

Vector Camera::GetPosition() const
{
	return position;
}

Vector Camera::GetForward() const
{
	return viewForward;
}

Vector Camera::GetUp() const
{
	return viewUp;
}

Vector Camera::GetRight() const
{
	return viewRight;
}

Vector Camera::GetLookAt() const
{
	return lookAt;
}

int Camera::GetScreenWidth() const
{
	return viewport.width;
}

int Camera::GetScreenHeight() const
{
	return viewport.height;
}

float Camera::GetFov() const
{
	return fov;
}

void Camera::SetFov(float fovDeg)
{
	fov = fovDeg;
}

float Camera::GetAspectRatio() const
{
	return aspectRatio;
}

void Camera::SetAspectRatio(float aspect)
{
	aspectRatio = aspect;
}

float Camera::GetNearPlane() const
{
	return nearPlane;
}

void Camera::SetNearPlane(float near)
{
	nearPlane = near;
}

float Camera::GetFarPlane() const
{
	return farPlane;
}

void Camera::SetFarPlane(float far)
{
	farPlane = far;
}

// Extents2D Camera::GetNearExtents() const
// {
// 	return Extents2D{ nearWidth, nearHeight };
// }
// 
// Extents2D Camera::GetFarExtents() const
// {
// 	return Extents2D{ farWidth, farHeight };
// }

IntRect Camera::GetCameraViewport() const
{
	return viewport;
}

