#pragma once

#include "Matrix.h"
#include "MathUtilities.h"

// TODO - Perspective and orthographic utilities sound nice, but also sound unnecessary. Think about where to possibly put these
namespace Math
{

	// Gets angle between 2 vectors
	// TODO - This should be a utility. Move somewhere
	inline float GetAngle(const Vector vec0, const Vector& vec1)
	{
		Vector v0 = vec0.GetNormalized();
		Vector v1 = vec1.GetNormalized();
		float cosAngle = v0.Dot(v1);
		return Math::Acos(cosAngle);
	}

	Matrix Perspective(float width, float height, float nearPlane, float farPlane);

	Matrix Orthographic(float aspect, float nearPlane, float farPlane);
}