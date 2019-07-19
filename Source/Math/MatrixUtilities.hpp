#pragma once

#include "Math/Matrix.h"

namespace Math
{

Matrix ConstructViewMatrix(const Vector& position, const Vector& lookAt, const Vector& up);
Matrix ConstructViewMatrix(const Vector& position, const Vector& forward, const Vector& right, const Vector& up);
Matrix ConstructPerspectiveMatrix(float32 fov, float32 aspectRatio, float32 nearPlane, float32 farPlane);
Matrix Orthographic(float32 aspect, float32 nearPlane, float32 farPlane);

}