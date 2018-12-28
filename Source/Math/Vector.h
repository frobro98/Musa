#pragma once

#include "MathDefinitions.h"
#include "MathConstants.h"

// TODO - Make this a struct instead of a class...
struct Vector
{
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
	float w = 1.f;

	Vector() = default;
	explicit Vector(float x, float y, float z, float w = 1.f);

	float Dot(const Vector& other) const;
	Vector Cross(const Vector& other) const;
	void Normalize();
	Vector GetNormalized() const;

	float Magnitude() const;
	float MagnitudeSqr() const;
	float InverseMagnitude() const;

	bool IsEqual(const Vector& other, float epsilon = Math::InternalTolerence) const;
	bool IsZero(float epsilon = Math::InternalTolerence) const;

	// TODO - get rid of this method?
	float GetAngle(const Vector& other) const;

	// Unary operators
	Vector operator+() const;
	Vector operator-() const;

	// Arithmetic operators
	Vector& operator+=(const Vector& other);
	Vector& operator-=(const Vector& other);

	Vector operator+(const Vector& other) const;
	Vector operator-(const Vector& other) const;

	Vector& operator*=(float s);
	Vector operator*(float s) const;
	friend Vector operator*(float s, const Vector & vec);

	friend bool operator==(const Vector& lhs, const Vector& rhs)
	{
		return lhs.IsEqual(rhs);
	}

	friend bool operator!=(const Vector& lhs, const Vector& rhs)
	{
		return !lhs.IsEqual(rhs);
	}

	friend bool operator>(const Vector& lhs, const Vector& rhs)
	{
		return lhs.x > rhs.x
			&& lhs.y > rhs.y
			&& lhs.z > rhs.z
			&& lhs.w > rhs.w;
	}

	friend bool operator>=(const Vector& lhs, const Vector& rhs)
	{
		return lhs.x >= rhs.x
			&& lhs.y >= rhs.y
			&& lhs.z >= rhs.z
			&& lhs.w >= rhs.w;
	}

	friend bool operator<(const Vector& lhs, const Vector& rhs)
	{
		return lhs.x < rhs.x
			&& lhs.y < rhs.y
			&& lhs.z < rhs.z
			&& lhs.w < rhs.w;
	}

	friend bool operator<=(const Vector& lhs, const Vector& rhs)
	{
		return lhs.x <= rhs.x
			&& lhs.y <= rhs.y
			&& lhs.z <= rhs.z
			&& lhs.w <= rhs.w;
	}
};
