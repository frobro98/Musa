#include "MatrixUtilities.hpp"
#include "MathUtilities.hpp"
#include "Assertion.h"

namespace Math
{
Matrix4 ConstructViewMatrix(const Vector4& position, const Vector4& lookAt, const Vector4& up)
{
	Vector4 viewForward = position - lookAt;
	viewForward.Normalize();

	Vector4 viewRight = up.Cross(viewForward);
	viewRight.Normalize();

	Vector4 viewUp = viewForward.Cross(viewRight);
	viewUp.Normalize();

	return ConstructViewMatrix(position, viewForward, viewRight, viewUp);
}
Matrix4 ConstructViewMatrix(const Vector4& position, const Vector4& forward, const Vector4& right, const Vector4& up)
{
	Matrix4 view;
	view[m0] = right.x;
	view[m1] = up.x;
	view[m2] = forward.x;
	view[m3] = 0.f;

	view[m4] = right.y;
	view[m5] = up.y;
	view[m6] = forward.y;
	view[m7] = 0.f;

	view[m8] = right.z;
	view[m9] = up.z;
	view[m10] = forward.z;
	view[m11] = 0.f;

	view[m12] = -position.Dot(right);
	view[m13] = -position.Dot(up);
	view[m14] = -position.Dot(forward);
	view[m15] = 1.f;

	return view;
}

Matrix4 ConstructPerspectiveMatrix(float32 fovDeg, float32 aspectRatio, float32 nearPlane, float32 farPlane)
{
	// NOTE: This is an OpenGL projection matrix. The adjustment happens in the shader currently

	float32 nearHeight = 2.f * Math::Tan(Math::DegreesToRadians(fovDeg) * .5f) * nearPlane;
	float32 nearWidth = nearHeight * aspectRatio;
	float32 doubleNearPlane = 2.f * nearPlane;
	float32 planeDifference = farPlane - nearPlane;

	Matrix4 projection;
	projection[m0] = doubleNearPlane / nearWidth;//1.f / (aspect * tanHalfFOV);
	projection[m1] = 0;
	projection[m2] = 0;
	projection[m3] = 0;

	projection[m4] = 0;
	projection[m5] = doubleNearPlane / nearHeight;// tanHalfFOV;
	projection[m6] = 0;
	projection[m7] = 0;

	projection[m8] = 0;
	projection[m9] = 0;
	projection[m10] = -(farPlane + nearPlane) / planeDifference;
	projection[m11] = -1.f;

	projection[m12] = 0;
	projection[m13] = 0;
	projection[m14] = -(2.f * farPlane * nearPlane) / planeDifference;
	projection[m15] = 0;

//	Assert((projection * ConstructInversePerspectiveMatrix(fovDeg, aspectRatio, nearPlane, farPlane)).IsEqual(Matrix(IDENTITY)));

	return projection;
}

Matrix4 ConstructOrthographicMatrix(float32 width, float32 height, float32 nearPlane, float32 farPlane)
{
	const float32 halfWidth = width * .5f;
	const float32 halfHeight = height * .5f;
	return ConstructOrthographicMatrix(-halfWidth, halfWidth, -halfHeight, halfHeight, nearPlane, farPlane);
}

Matrix4 ConstructOrthographicMatrix(float32 left, float32 right, float32 bottom, float32 top, float32 nearPlane, float32 farPlane)
{
	// NOTE: This is an OpenGL projection matrix. The adjustment happens in the shader currently
	Matrix4 projection;
	projection[m0] = 2.f / (right - left);
	projection[m1] = 0;
	projection[m2] = 0;
	projection[m3] = -(right + left) / (right - left);

	projection[m4] = 0;
	projection[m5] = 2.f / (top - bottom);
	projection[m6] = 0;
	projection[m7] = -(top + bottom) / (top - bottom);

	projection[m8] = 0;
	projection[m9] = 0;
	projection[m10] = -2.f / (farPlane - nearPlane);
	projection[m11] = -(farPlane + nearPlane) / (farPlane - nearPlane);

	projection[m12] = 0;
	projection[m13] = 0;
	projection[m14] = 0;
	projection[m15] = 1;

	Assert((projection * ConstructFastInverseOrthographicMatrix(left, right, bottom, top, nearPlane, farPlane)).IsEqual(Matrix4(IDENTITY)));

	return projection;
}

Matrix4 ConstructScreenTransformMatrix(float32 screenWidth, float32 screenHeight, float32 worldDepth)
{
	float32 halfScreenWidth = screenWidth * .5f;
	float32 halfScreeHeight = screenHeight * .5f;
	float32 halfDepth = worldDepth * .5f;
	Matrix4 projection;
	projection[m0] = halfScreenWidth;
	projection[m1] = 0;
	projection[m2] = 0;
	projection[m3] = 0;

	projection[m4] = 0;
	projection[m5] = -halfScreeHeight;
	projection[m6] = 0;
	projection[m7] = 0;

	projection[m8] = 0;
	projection[m9] = 0;
	projection[m10] = halfDepth;
	projection[m11] = 0;

	projection[m12] = halfScreenWidth;
	projection[m13] = halfScreeHeight;
	projection[m14] = halfDepth;
	projection[m15] = 1;

	Assert((projection * ConstructFastInverseScreenMatrix(screenWidth, screenHeight, worldDepth)).IsEqual(Matrix4(IDENTITY)));

	return projection;
}

// Matrix ConstructInversePerspectiveMatrix(float32 fovDeg, float32 aspectRatio, float32 nearPlane, float32 farPlane)
// {
// 	// NOTE: This is an OpenGL projection matrix. The adjustment happens in the shader currently
// 
// 	float32 nearHeight = 2.f * Math::Tan(Math::DegreesToRadians(fovDeg) * .5f) * nearPlane;
// 	float32 nearWidth = nearHeight * aspectRatio;
// 
// 	float32 doubleNearPlane = 2.f * nearPlane;
// 
// // 	Matrix projection;
// // 	projection[m0] = nearWidth / doubleNearPlane;
// // 	projection[m1] = 0;
// // 	projection[m2] = 0;
// // 	projection[m3] = 0;
// // 
// // 	projection[m4] = 0;
// // 	projection[m5] = nearHeight / doubleNearPlane;
// // 	projection[m6] = 0;
// // 	projection[m7] = 0;
// // 
// // 	projection[m8] = 0;
// // 	projection[m9] = 0;
// // 	projection[m10] = 0;
// // 	projection[m11] = (nearPlane - farPlane) / (2.f * farPlane * nearPlane);
// // 
// // 	projection[m12] = 0;
// // 	projection[m13] = 0;
// // 	projection[m14] = -1.f;
// // 	projection[m15] = (farPlane - nearPlane) / (2.f * farPlane * nearPlane);
// 
// 	return projection;
// }

Matrix4 ConstructFastInverseOrthographicMatrix(float32 width, float32 height, float32 nearPlane, float32 farPlane)
{
	const float32 halfWidth = width * .5f;
	const float32 halfHeight = height * .5f;
	return ConstructFastInverseOrthographicMatrix(-halfWidth, halfWidth, -halfHeight, halfHeight, nearPlane, farPlane);
}

Matrix4 ConstructFastInverseOrthographicMatrix(float32 left, float32 right, float32 bottom, float32 top, float32 nearPlane, float32 farPlane)
{
	Matrix4 projection;
	projection[m0] = (right - left) * .5f;
	projection[m1] = 0;
	projection[m2] = 0;
	projection[m3] = (right + left) * .5f;

	projection[m4] = 0;
	projection[m5] = (top - bottom) * .5f;
	projection[m6] = 0;
	projection[m7] = (top + bottom) * .5f;

	projection[m8] = 0;
	projection[m9] = 0;
	projection[m10] = -(farPlane - nearPlane) * .5f;
	projection[m11] = -(farPlane + nearPlane) * .5f;

	projection[m12] = 0;
	projection[m13] = 0;
	projection[m14] = 0;
	projection[m15] = 1;

	//Assert((projection * ConstructOrthographicMatrix(left, right, bottom, top, nearPlane, farPlane)).IsEqual(Matrix(IDENTITY)));
	return projection;
}

Matrix4 ConstructFastInverseScreenMatrix(float32 screenWidth, float32 screenHeight, float32 worldDepth)
{
	Matrix4 projection;
	projection[m0] = 2.f / screenWidth;
	projection[m1] = 0;
	projection[m2] = 0;
	projection[m3] = 0;

	projection[m4] = 0;
	projection[m5] = -2.f / screenHeight;
	projection[m6] = 0;
	projection[m7] = 0;

	projection[m8] = 0;
	projection[m9] = 0;
	projection[m10] = 2.f / worldDepth;
	projection[m11] = 0;

	projection[m12] = -1.f;
	projection[m13] = 1.f;
	projection[m14] = -1.f;
	projection[m15] = 1;

	return projection;
}

}