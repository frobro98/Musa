#pragma once

#include "Math/MathEngine.h"
#include "Model/GeometryPrimitives.h"
#include "Texture/Color.hpp"

class GameObject;

struct Viewport
{
	int32 x;
	int32 y;
	int32 width;
	int32 height;
	//Color clearColor;
};

struct Frustum
{
	// near points
	Vector nearTopLeft;
	Vector nearTopRight;
	Vector nearBottomLeft;
	Vector nearBottomRight;

	// far points
	Vector farTopLeft;
	Vector farTopRight;
	Vector farBottomLeft;
	Vector farBottomRight;

	// Normals for collision
	Vector nearPlaneNormal;
	Vector farPlaneNormal;
	Vector topNormal;
	Vector bottomNormal;
	Vector leftNormal;
	Vector rightNormal;
};

class Camera final
{
public:
	Camera() = default;
	~Camera() = default;
	Camera(const Camera&) = default;

	void SetPerspective(float fovDeg, float aspectRatio, float nearPlane, float farPlane);
	void SetViewport(const Viewport& vp);
	void SetOrientationAndPosition(const Vector& lookAt, const Vector& pos, const Vector& up);

	void Update();

	Matrix& GetView();
	// TODO - move to a more logical location, like the viewport or something?
	Matrix& GetProjection();

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

	Frustum GetCameraFrustum();
	Viewport GetCameraViewport();

	void SetCollisionCheck(GameObject* gameObject);

private: // Functions
	void CalculatePlaneDimentions();
	void CalculateFrustum();
	void CalculateFrustumNormals();
	void UpdateView();
	void UpdateProjection();

	void CalculateCollision();

	void UpdateViewport();

private:
	Matrix projection;
	Matrix view;

	// view matrix
	Vector viewRight;
	Vector viewUp;
	Vector viewForward;
	
	Vector position;
	Vector lookAt;

	Frustum frustum;

	Viewport viewport;

	Color clearColor{ Color::Black() };

	GameObject* objCollision = nullptr;

	float nearPlane;
	float farPlane;
	float fov;
	float aspectRatio;

	// Projection related stuff
	// TODO - move this data wherever you move the projection matrix
	float nearWidth;
	float nearHeight;
	float farWidth;
	float farHeight;

	uint32 pad[2] = { 0, 0 };
};