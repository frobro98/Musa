// Copyright 2020, Nathan Blane

#pragma once

#include "Math/Matrix4.hpp"
#include "Math/MathAPI.hpp"

namespace Math
{

MATH_API Matrix4 ConstructViewMatrix(const Vector4& position, const Vector4& lookAt, const Vector4& up);
MATH_API Matrix4 ConstructViewMatrix(const Vector4& position, const Vector4& forward, const Vector4& right, const Vector4& up);
MATH_API Matrix4 ConstructPerspectiveMatrix(f32 fov, f32 aspectRatio, f32 nearPlane, f32 farPlane);
MATH_API Matrix4 ConstructOrthographicMatrix(f32 width, f32 height, f32 nearPlane, f32 farPlane);
MATH_API Matrix4 ConstructOrthographicMatrix(f32 left, f32 right, f32 bottom, f32 top, f32 nearPlane, f32 farPlane);
MATH_API Matrix4 ConstructScreenTransformMatrix(f32 screenWidth, f32 screenHeight, f32 worldDepth = 1.f);

//Matrix ConstructInversePerspectiveMatrix(float32 fov, float32 aspectRatio, float32 nearPlane, float32 farPlane);
MATH_API Matrix4 ConstructFastInverseOrthographicMatrix(f32 width, f32 height, f32 nearPlane, f32 farPlane);
MATH_API Matrix4 ConstructFastInverseOrthographicMatrix(f32 left, f32 right, f32 bottom, f32 top, f32 nearPlane, f32 farPlane);
MATH_API Matrix4 ConstructFastInverseScreenMatrix(f32 screenWidth, f32 screenHeight, f32 worldDepth = 1.f);

}