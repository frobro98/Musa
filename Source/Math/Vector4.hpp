#pragma once

#include "Types/Intrinsics.hpp"
#include "Math/MathConstants.hpp"

struct Vector2;
struct Vector3;

struct Vector4
{
	static const Vector4 RightAxis;
	static const Vector4 UpAxis;
	static const Vector4 ForwardAxis;
	static const Vector4 Zero;
	static const Vector4 One;

public:
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
	float w = 1.f;

	Vector4() = default;
	explicit Vector4(float x, float y, float z, float w = 1.f);
	explicit Vector4(const Vector3& v, float32 w = 1.f);
	explicit Vector4(const Vector2& v, float32 z, float32 w = 1.f);

	float Dot(const Vector4& other) const;
	Vector4 Cross(const Vector4& other) const;
	void Normalize();
	Vector4 GetNormalized() const;

	float Magnitude() const;
	float MagnitudeSqr() const;
	float InverseMagnitude() const;

	bool IsEqual(const Vector4& other, float epsilon = Math::InternalTolerence) const;
	bool IsZero(float epsilon = Math::InternalTolerence) const;


	// Unary operators
	Vector4 operator+() const;
	Vector4 operator-() const;

	// Arithmetic operators
	Vector4& operator+=(const Vector4& other);
	Vector4& operator-=(const Vector4& other);

	Vector4 operator+(const Vector4& other) const;
	Vector4 operator-(const Vector4& other) const;

	Vector4& operator*=(float s);
	Vector4 operator*(float s) const;
	friend Vector4 operator*(float s, const Vector4 & vec);

	friend bool operator==(const Vector4& lhs, const Vector4& rhs)
	{
		return lhs.IsEqual(rhs);
	}

	friend bool operator!=(const Vector4& lhs, const Vector4& rhs)
	{
		return !lhs.IsEqual(rhs);
	}

	friend bool operator>(const Vector4& lhs, const Vector4& rhs)
	{
		return lhs.x > rhs.x
			&& lhs.y > rhs.y
			&& lhs.z > rhs.z
			&& lhs.w > rhs.w;
	}

	friend bool operator>=(const Vector4& lhs, const Vector4& rhs)
	{
		return lhs.x >= rhs.x
			&& lhs.y >= rhs.y
			&& lhs.z >= rhs.z
			&& lhs.w >= rhs.w;
	}

	friend bool operator<(const Vector4& lhs, const Vector4& rhs)
	{
		return lhs.x < rhs.x
			&& lhs.y < rhs.y
			&& lhs.z < rhs.z
			&& lhs.w < rhs.w;
	}

	friend bool operator<=(const Vector4& lhs, const Vector4& rhs)
	{
		return lhs.x <= rhs.x
			&& lhs.y <= rhs.y
			&& lhs.z <= rhs.z
			&& lhs.w <= rhs.w;
	}
};