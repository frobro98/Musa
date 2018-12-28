#pragma once

struct Vector;

namespace Internal
{

// TODO - This is where the switch will be to actually implement SIMD vs regular

float VectorDotVector(const Vector& v0, const Vector& v1);
void VectorCrossVector(const Vector& v0, const Vector& v1, Vector& perpVector);
void VectorNormalize(Vector& v);
float VectorMagnitudeSqr(const Vector& v);
float VectorMagnitude(const Vector& v);
float VectorInverseMagnitude(const Vector& v);
float VectorAngleBetweenVectors(const Vector& v0, const Vector& v1);

// Vector Math
void VectorAddTo(Vector& v0, const Vector& v1);
void VectorSubTo(Vector& v0, const Vector& v1);
void VectorMultiTo(Vector& v0, float s);
void VectorDivTo(Vector& v0, float s);

}