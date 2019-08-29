#include "MatrixUtilities.hpp"
#include "MathUtilities.h"

namespace Math
{
Matrix ConstructViewMatrix(const Vector4& position, const Vector4& lookAt, const Vector4& up)
{
	Vector4 viewForward = position - lookAt;
	viewForward.Normalize();

	Vector4 viewRight = up.Cross(viewForward);
	viewRight.Normalize();

	Vector4 viewUp = viewForward.Cross(viewRight);
	viewUp.Normalize();

	return ConstructViewMatrix(position, viewForward, viewRight, viewUp);
}
Matrix ConstructViewMatrix(const Vector4& position, const Vector4& forward, const Vector4& right, const Vector4& up)
{
	Matrix view;
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

Matrix ConstructPerspectiveMatrix(float32 fovDeg, float32 aspectRatio, float32 nearPlane, float32 farPlane)
{
	float32 nearHeight = 2.f * Math::Tan(Math::DegreesToRadians(fovDeg) * .5f) * nearPlane;
	float32 nearWidth = nearHeight * aspectRatio;

	Matrix projection;
	projection[m0] = 2.f * nearPlane / nearWidth;//1.f / (aspect * tanHalfFOV);
	projection[m1] = 0;
	projection[m2] = 0;
	projection[m3] = 0;

	projection[m4] = 0;
	projection[m5] = 2.f * nearPlane / nearHeight;// tanHalfFOV;
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

	return projection;
}

Matrix ConstructOrthographicMatrix(float32 left, float32 right, float32 bottom, float32 top, float32 nearPlane, float32 farPlane)
{
	// NOTE: This is an OpenGL projection matrix. The adjustment happens in the shader currently
	Matrix projection;
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
	projection[m11] = -nearPlane / (farPlane - nearPlane);

	projection[m12] = 0;
	projection[m13] = 0;
	projection[m14] = 0;
	projection[m15] = 1;

	return projection;
}

}