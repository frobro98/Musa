
#include "Camera.h"
#include "GameObject/GameObject.h"
#include "Model/Mesh.h"

#undef near
#undef far

void Camera::SetPerspective(float fovDeg, float aspect, float near, float far)
{
	aspectRatio = aspect;
	fov = fovDeg;
	nearPlane = near;
	farPlane = far;
}

void Camera::SetViewport(const Viewport& vp)
{
	viewport = vp;
	UpdateViewport();
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

void Camera::Update()
{
	UpdateViewport();

	CalculatePlaneDimentions();

	CalculateFrustum();

	UpdateProjection();
	UpdateView();

	if (objCollision)
	{
		CalculateCollision();
	}
}

void Camera::CalculatePlaneDimentions()
{
	nearHeight = 2.f * Math::Tan(Math::DegreesToRadians(fov) * .5f) * nearPlane;
	nearWidth = nearHeight * aspectRatio;
	farHeight = 2.f * Math::Tan(Math::DegreesToRadians(fov) * .5f) * farPlane;
	farWidth = farHeight * aspectRatio;
}

void Camera::CalculateFrustum()
{
	frustum.nearTopLeft = position - viewForward * this->nearPlane + this->viewUp * this->nearHeight * 0.5f - this->viewRight * this->nearWidth * 0.5f;
	frustum.nearTopRight = this->position - this->viewForward * this->nearPlane + this->viewUp * this->nearHeight * 0.5f + this->viewRight * this->nearWidth * 0.5f;
	frustum.nearBottomLeft = this->position - this->viewForward * this->nearPlane - this->viewUp * this->nearHeight * 0.5f - this->viewRight * this->nearWidth * 0.5f;
	frustum.nearBottomRight = this->position - this->viewForward * this->nearPlane - this->viewUp * this->nearHeight * 0.5f + this->viewRight * this->nearWidth * 0.5f;
	
	frustum.farTopLeft = this->position - this->viewForward * this->farPlane + this->viewUp * this->farHeight * 0.5f - this->viewRight * this->farWidth * 0.5f;
	frustum.farTopRight = this->position - this->viewForward * this->farPlane + this->viewUp * this->farHeight * 0.5f + this->viewRight * this->farWidth * 0.5f;
	frustum.farBottomLeft = this->position - this->viewForward * this->farPlane - this->viewUp * this->farHeight * 0.5f - this->viewRight * this->farWidth * 0.5f;
	frustum.farBottomRight = this->position - this->viewForward * this->farPlane - this->viewUp * this->farHeight * 0.5f + this->viewRight * this->farWidth * 0.5f;

	CalculateFrustumNormals();
}
// TODO - this might be good in the frustum structure
void Camera::CalculateFrustumNormals()
{
	Vector A = frustum.nearBottomLeft - frustum.nearTopLeft;
	Vector B = frustum.nearTopRight - frustum.nearTopLeft;
	Vector C = frustum.farTopLeft - frustum.nearTopLeft;

	frustum.nearPlaneNormal = A.Cross(B);
	frustum.nearPlaneNormal.Normalize();

	frustum.leftNormal = C.Cross(A);
	frustum.leftNormal.Normalize();

	frustum.topNormal = B.Cross(C);
	frustum.topNormal.Normalize();

	A = frustum.farBottomLeft - frustum.farBottomRight;
	B = frustum.farTopRight - frustum.farBottomRight;
	C = frustum.nearBottomRight - frustum.farBottomRight;

	frustum.farPlaneNormal = A.Cross(B);
	frustum.farPlaneNormal.Normalize();

	frustum.rightNormal = B.Cross(C);
	frustum.rightNormal.Normalize();

	frustum.bottomNormal = C.Cross(A);
	frustum.bottomNormal.Normalize();
}

void Camera::UpdateView()
{
	view[m0] = viewRight.x;
	view[m1] = viewUp.x;
	view[m2] = viewForward.x;
	view[m3] = 0.f;

	view[m4] = viewRight.y;
	view[m5] = viewUp.y;
	view[m6] = viewForward.y;
	view[m7] = 0.f;

	view[m8] = viewRight.z;
	view[m9] = viewUp.z;
	view[m10] = viewForward.z;
	view[m11] = 0.f;

	view[m12] = -position.Dot(viewRight);
	view[m13] = -position.Dot(viewUp);
	view[m14] = -position.Dot(viewForward);
	view[m15] = 1.f;
}

void Camera::UpdateProjection()
{
	projection[m0] = 2.f * nearPlane / nearWidth;//1.f / (aspect * tanHalfFOV);
	projection[m1] = 0;
	projection[m2] = 0;
	projection[m3] = 0;

	projection[m4] = 0;
	projection[m5] = 2.f * nearPlane / nearHeight;//1.f / tanHalfFOV;
	projection[m6] = 0;
	projection[m7] = 0;

	projection[m8] = 0;
	projection[m9] = 0;
	projection[m10] = (farPlane + nearPlane) / (nearPlane - farPlane);//-(farPlane + nearPlane) / (farPlane - nearPlane);
	projection[m11] = -1.f;

	projection[m12] = 0;
	projection[m13] = 0;
	projection[m14] = (2.f * farPlane * nearPlane) / (nearPlane - farPlane);
	projection[m15] = 0;

}

void Camera::UpdateViewport()
{
	// TODO - need to update viewport somewhere else. Probably in a separate class that will contain viewport parts
	//glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
}

Matrix& Camera::GetView()
{
	return view;
}

Matrix & Camera::GetProjection()
{
	return projection;
}

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

Frustum Camera::GetCameraFrustum()
{
	return frustum;
}

Viewport Camera::GetCameraViewport()
{
	return viewport;
}

void Camera::SetCollisionCheck(GameObject * gameObject)
{
	objCollision = gameObject;
}

void Camera::CalculateCollision()
{
	CollisionInfo info = objCollision->GetCollisionInfo();
	Vector pos = info.boundingSphere.cntr;
	float radius = info.boundingSphere.rad;

	// Near Plane Test
	Vector vec = pos - frustum.nearBottomRight;
	float nearDist = vec.Dot(frustum.nearPlaneNormal);

	// Far Plane Test
	vec = pos - frustum.farTopLeft;
	float farDist = vec.Dot(frustum.farPlaneNormal);

	// Left Plane Test
	vec = pos - frustum.farTopLeft;
	float leftDist = vec.Dot(frustum.leftNormal);

	// Right Plane Test
	vec = pos - frustum.nearBottomRight;
	float rightDist = vec.Dot(frustum.rightNormal);

	// Top Plane Test
	vec = pos - frustum.farTopLeft;
	float topDist = vec.Dot(frustum.topNormal);

	// Bottom Plane Test
	vec = pos - frustum.nearBottomRight;
	float bottomDist = vec.Dot(frustum.bottomNormal);

	//printf("Bottom:%f Top:%f Left:%f Right:%f Near:%f Far:%f\n", bottomDist, topDist, leftDist, rightDist, nearDist, farDist);

	if (
		nearDist < radius &&
		farDist < radius &&
		leftDist < radius &&
		rightDist < radius &&
		topDist < radius && 
		bottomDist < radius
		)
	{
		objCollision->SetDebugColor(Color(1, 0, 0, 1));
	}
	else
	{
		objCollision->SetDebugColor(Color(0, 1, 0, 1));
	}
}
