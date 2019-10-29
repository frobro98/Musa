#include "IntVector2.hpp"
#include "Vector2.hpp"

IntVector2::IntVector2(int32 x_, int32 y_)
	: x(x_),
	y(y_)
{
}

IntVector2::IntVector2(const Vector2& v)
	: x((int32)v.x),
	y((int32)v.y)
{
}

bool IntVector2::IsZero() const
{
	return x == 0 && y == 0;
}

IntVector2 IntVector2::operator+() const
{
	return IntVector2(+x, +y);
}

IntVector2 IntVector2::operator-() const
{
	return IntVector2(-x, -y);
}

IntVector2& IntVector2::operator+=(const IntVector2& other)
{
	x += other.x;
	y += other.y;
	return *this;
}

IntVector2& IntVector2::operator-=(const IntVector2& other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

IntVector2& IntVector2::operator*=(int32 scalar)
{
	x *= scalar;
	y *= scalar;
	return *this;
}

IntVector2& IntVector2::operator/=(int32 scalar)
{
	x /= scalar;
	y /= scalar;
	return *this;
}

