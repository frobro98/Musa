// Copyright 2020, Nathan Blane

#pragma once

#include "Types/Intrinsics.hpp"
#include "Math/MathConstants.hpp"

struct Vector2;
struct Vector4;

struct Vector3
{
	static const Vector3 RightAxis;
	static const Vector3 UpAxis;
	static const Vector3 ForwardAxis;
	static const Vector3 Zero;
	static const Vector3 One;

public:
	float32 x = 0.f;
	float32 y = 0.f;
	float32 z = 0.f;

	Vector3() = default;
	explicit Vector3(float32 x, float32 y, float32 z);
	explicit Vector3(const Vector4& v4);
	explicit Vector3(const Vector2& v2, float32 inZ);

	float Dot(const Vector3& other) const;
	Vector3 Cross(const Vector3& other) const;
	void Normalize();
	Vector3 GetNormalized() const;

	float Magnitude() const;
	float MagnitudeSqr() const;
	float InverseMagnitude() const;

	bool IsEqual(const Vector3& other, float epsilon = Math::InternalTolerence) const;
	bool IsZero(float epsilon = Math::InternalTolerence) const;


	// Unary operators
	Vector3 operator+() const;
	Vector3 operator-() const;

	// Arithmetic operators
	Vector3& operator+=(const Vector3& other);
	Vector3& operator-=(const Vector3& other);

	Vector3 operator+(const Vector3& other) const;
	Vector3 operator-(const Vector3& other) const;

	Vector3& operator*=(float32 s);
	Vector3 operator*(float32 s) const;
	friend Vector3 operator*(float32 s, const Vector3 & vec);

	friend bool operator==(const Vector3& lhs, const Vector3& rhs)
	{
		return lhs.IsEqual(rhs);
	}

	friend bool operator!=(const Vector3& lhs, const Vector3& rhs)
	{
		return !lhs.IsEqual(rhs);
	}

	friend bool operator>(const Vector3& lhs, const Vector3& rhs)
	{
		return lhs.x > rhs.x
			&& lhs.y > rhs.y
			&& lhs.z > rhs.z;
	}

	friend bool operator>=(const Vector3& lhs, const Vector3& rhs)
	{
		return lhs.x >= rhs.x
			&& lhs.y >= rhs.y
			&& lhs.z >= rhs.z;
	}

	friend bool operator<(const Vector3& lhs, const Vector3& rhs)
	{
		return lhs.x < rhs.x
			&& lhs.y < rhs.y
			&& lhs.z < rhs.z;
	}

	friend bool operator<=(const Vector3& lhs, const Vector3& rhs)
	{
		return lhs.x <= rhs.x
			&& lhs.y <= rhs.y
			&& lhs.z <= rhs.z;
	}
};
