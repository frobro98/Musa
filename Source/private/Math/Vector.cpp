
#include "MathEngine.h"
#include "MathUtilities.h"
#include "Internal/VectorImplementation.h"

const Vector Vector::RightAxis(1, 0, 0);
const Vector Vector::UpAxis(0, 1, 0);
const Vector Vector::ForwardAxis(0, 0, 1);
const Vector Vector::Zero(0, 0, 0);
const Vector Vector::One(1, 1, 1);

Vector::Vector(float x_, float y_, float z_, float w_)
	: x(x_), y(y_), z(z_), w(w_)
{
}

float Vector::Dot(const Vector& other) const
{
	return Internal::VectorDotVector(*this, other);
}

Vector Vector::Cross(const Vector& other) const
{
	Vector out;
	Internal::VectorCrossVector(*this, other, out);
	return out;
}

void Vector::Normalize()
{
	Internal::VectorNormalize(*this);
}

Vector Vector::GetNormalized() const
{
	Vector ret(*this);
	Internal::VectorNormalize(ret);
	return ret;
}

float Vector::Magnitude() const
{
	return Internal::VectorMagnitude(*this);
}

float Vector::MagnitudeSqr() const
{
	return Internal::VectorMagnitudeSqr(*this);
}

float Vector::InverseMagnitude() const
{
	return Internal::VectorInverseMagnitude(*this);
}

bool Vector::IsEqual(const Vector& other, float epsilon) const
{
	return (Math::IsEqual(x, other.x, epsilon) && 
			Math::IsEqual(y, other.y, epsilon) &&
			Math::IsEqual(z, other.z, epsilon) &&
			Math::IsEqual(w, other.w, epsilon));
}

bool Vector::IsZero(float epsilon) const
{
	return (Math::IsZero(x, epsilon) &&
			Math::IsZero(y, epsilon) && 
			Math::IsZero(z, epsilon) && 
			Math::IsOne(w, epsilon));
}

// float Vector::GetAngle(const Vector& other) const
// {
// 	return Internal::VectorAngleBetweenVectors(*this, other);
// }

Vector Vector::operator+() const
{
	return Vector(+x, +y, +z);
}

Vector Vector::operator-() const
{
	return Vector(-x, -y, -z);
}

Vector& Vector::operator+=(const Vector& other)
{
	Internal::VectorAddTo(*this, other);
	return *this;
}

Vector& Vector::operator-=(const Vector& other)
{
	Internal::VectorSubTo(*this, other);
	return *this;
}

Vector Vector::operator+(const Vector & other) const
{
	Vector v(*this);
	return v += other;
}

Vector Vector::operator-(const Vector & other) const
{

	Vector v(*this);
	return v -= other;
}

Vector & Vector::operator*=(float s)
{
	Internal::VectorMultiTo(*this, s);
	return *this;
}

Vector Vector::operator*(float s) const
{
	Vector v(*this);
	v *= s;
	return v;
}

Vector operator*(float s, const Vector & vec)
{
	Vector v(vec);
	v *= s;
	return v;
}
