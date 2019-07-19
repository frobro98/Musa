#pragma once

#include "EngineCore/Extents.hpp"
#include "Math/Vector.h"
#include "Math/Matrix.h"

struct SphereBounds;

class ViewFrustum
{
	ViewFrustum(
		const Matrix& viewMatrix,
		const Vector& origin,  
		const Extents2D& nearExtents,
		const Extents2D& farExtents,
		float32 nearPlane,
		float32 farPlane
	);

	bool DoesSphereIntersect(const SphereBounds& bounds);

private:
	void CalculateCollisionNormals();

private:
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