#pragma once

struct Vector4;

namespace Internal
{

// NOTE - This is where the switch will be to actually implement SIMD vs regular

float VectorDotVector(const Vector4& v0, const Vector4& v1);
void VectorCrossVector(const Vector4& v0, const Vector4& v1, Vector4& perpVector);
void VectorNormalize(Vector4& v);
float VectorMagnitudeSqr(const Vector4& v);
float VectorMagnitude(const Vector4& v);
float VectorInverseMagnitude(const Vector4& v);
float VectorAngleBetweenVectors(const Vector4& v0, const Vector4& v1);

// Vector Math
void VectorAddTo(Vector4& v0, const Vector4& v1);
void VectorSubTo(Vector4& v0, const Vector4& v1);
void VectorMultiTo(Vector4& v0, float s);
void VectorDivTo(Vector4& v0, float s);

}