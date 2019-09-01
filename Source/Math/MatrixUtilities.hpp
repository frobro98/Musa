#pragma once

#include "Math/Matrix.h"

namespace Math
{

Matrix ConstructViewMatrix(const Vector4& position, const Vector4& lookAt, const Vector4& up);
Matrix ConstructViewMatrix(const Vector4& position, const Vector4& forward, const Vector4& right, const Vector4& up);
Matrix ConstructPerspectiveMatrix(float32 fov, float32 aspectRatio, float32 nearPlane, float32 farPlane);
Matrix ConstructOrthographicMatrix(float32 width, float32 height, float32 nearPlane, float32 farPlane);
Matrix ConstructOrthographicMatrix(float32 left, float32 right, float32 bottom, float32 top, float32 nearPlane, float32 farPlane);
Matrix ConstructScreenTransformMatrix(float32 screenWidth, float32 screenHeight, float32 worldDepth = 1.f);

//Matrix ConstructInversePerspectiveMatrix(float32 fov, float32 aspectRatio, float32 nearPlane, float32 farPlane);
Matrix ConstructFastInverseOrthographicMatrix(float32 width, float32 height, float32 nearPlane, float32 farPlane);
Matrix ConstructFastInverseOrthographicMatrix(float32 left, float32 right, float32 bottom, float32 top, float32 nearPlane, float32 farPlane);
Matrix ConstructFastInverseScreenMatrix(float32 screenWidth, float32 screenHeight, float32 worldDepth = 1.f);

}