
#include "VectorImplementation.h"
#include "Assertion.h"
#include "Vector.h"
#include "MathUtilities.h"

#include "PlatformDefinitions.h"
// Includes to handle SIMD register types
WALL_WRN_PUSH
#include <xmmintrin.h>
#include <smmintrin.h> 
WALL_WRN_POP

namespace Internal
{
// TODO - Probably should be in a different file but not now...
#ifdef VECTOR_ALGO_SSE
using VectorRegister = __m128;

#else
float VectorDotVector(const Vector& v0, const Vector& v1)
{
	return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
}

void VectorCrossVector(const Vector& v0, const Vector& v1, Vector& perpVector)
{
	perpVector = Vector(
		v0.y * v1.z - v1.y * v0.z,
		v0.z * v1.x - v1.z * v0.x,
		v0.x * v1.y - v1.x * v0.y
	);
}

void VectorNormalize(Vector& v)
{
	Assert(!v.IsZero());
	float invMag = VectorInverseMagnitude(v);
	v.x *= invMag;
	v.y *= invMag;
	v.z *= invMag;
}

float VectorMagnitudeSqr(const Vector& v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

float VectorMagnitude(const Vector& v)
{
	return Math::Sqrt(VectorMagnitudeSqr(v));
}

float VectorInverseMagnitude(const Vector& v)
{
	return 1.f / VectorMagnitude(v);
}

float VectorAngleBetweenVectors(const Vector& v0, const Vector& v1)
{
	Vector v0Norm = v0;
	Vector v1Norm = v1;
	VectorNormalize(v0Norm);
	VectorNormalize(v1Norm);
	float cosAngle = VectorDotVector(v0Norm, v1Norm);
	return Math::Acos(cosAngle);
}

// Vector Math
void VectorAddTo(Vector& v0, const Vector& v1)
{
	v0.x += v1.x;
	v0.y += v1.y;
	v0.z += v1.z;
	v0.w = 1.f;
}

void VectorSubTo(Vector& v0, const Vector& v1)
{
	v0.x -= v1.x;
	v0.y -= v1.y;
	v0.z -= v1.z;
	v0.w = 1.f;
}

void VectorMultiTo(Vector& v0, float s)
{
	v0.x *= s;
	v0.y *= s;
	v0.z *= s;
	v0.w = 1.f;
}

void VectorDivTo(Vector& v0, float s)
{
	float invS = 1.f / s;
	v0.x *= invS;
	v0.y *= invS;
	v0.z *= invS;
	v0.w = 1.f;
}

#endif // VECTOR_ALGO_SSE

}
