#pragma once

#include "Types/Intrinsics.hpp"

struct Vector2;

struct IntVector2
{
	int32 x = 0;
	int32 y = 0;

	IntVector2() = default;
	explicit IntVector2(int32 x_, int32 y_);
	explicit IntVector2(const Vector2& v);

	bool IsZero() const;

	IntVector2 operator+() const;
	IntVector2 operator-() const;

	IntVector2& operator+=(const IntVector2& other);
	IntVector2& operator-=(const IntVector2& other);
	IntVector2& operator*=(int32 scalar);
	IntVector2& operator/=(int32 scalar);

	friend IntVector2 operator+(const IntVector2& v0, const IntVector2& v1)
	{
		IntVector2 ret(v0);
		ret += v1;
		return ret;
	}

	friend IntVector2 operator-(const IntVector2& v0, const IntVector2& v1)
	{
		IntVector2 ret(v0);
		ret -= v1;
		return ret;
	}

	friend IntVector2 operator*(const IntVector2& v, int32 scalar)
	{
		IntVector2 ret(v);
		ret *= scalar;
		return ret;
	}

	friend IntVector2 operator*(int32 scalar, const IntVector2& v)
	{
		IntVector2 ret(v);
		ret *= scalar;
		return ret;
	}

	friend IntVector2 operator/(const IntVector2& v, int32 scalar)
	{
		IntVector2 ret(v);
		ret /= scalar;
		return ret;
	}

	friend IntVector2 operator/(int32 scalar, const IntVector2& v)
	{
		IntVector2 ret(v);
		ret /= scalar;
		return ret;
	}

	friend bool operator==(const IntVector2& v0, const IntVector2& v1)
	{
		return v0.x == v1.x &&
			v0.y == v1.y;
	}

	friend bool operator!=(const IntVector2& v0, const IntVector2& v1)
	{
		return v0.x != v1.x &&
			v0.y != v1.y;
	}

	friend bool operator>(const IntVector2& v0, const IntVector2& v1)
	{
		return v0.x > v1.x &&
			v0.y > v1.y;
	}

	friend bool operator<(const IntVector2& v0, const IntVector2& v1)
	{
		return v0.x < v1.x &&
			v0.y < v1.y;
	}

	friend bool operator>=(const IntVector2& v0, const IntVector2& v1)
	{
		return v0.x >= v1.x &&
			v0.y >= v1.y;
	}

	friend bool operator<=(const IntVector2& v0, const IntVector2& v1)
	{
		return v0.x <= v1.x &&
			v0.y <= v1.y;
	}
};

