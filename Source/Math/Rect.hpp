#pragma once

#include "EngineCore/Types/Intrinsics.hpp"
#include "EngineCore/Assertion.h"
#include "EngineCore/Extents.hpp"

struct IntRect;

struct Rect
{
	float32 x = 0;
	float32 y = 0;
	float32 width = 0;
	float32 height = 0;

	constexpr Rect() = default;

	constexpr Rect(float32 x_, float32 y_, float32 width_, float32 height_)
		: x(x_), y(y_), width(width_), height(height_)
	{
	}
};

struct IntRect
{
	int32 x = 0;
	int32 y = 0;
	int32 width = 0;
	int32 height = 0;

	constexpr IntRect() = default;

	constexpr IntRect(int32 x_, int32 y_, int32 width_, int32 height_)
		: x(x_), y(y_), width(width_), height(height_)
	{
	}
};

struct NormalizedRect
{

	float32 x = 0;
	float32 y = 0;
	float32 width = 0;
	float32 height = 0;

	constexpr NormalizedRect() = default;

	constexpr NormalizedRect(float32 x_, float32 y_, float32 width_, float32 height_)
		: x(x_), y(y_), width(width_), height(height_)
	{
		Assert(x_ >= 0.f && x_ <= 1.f);
		Assert(y_ >= 0.f && y_ <= 1.f);
		Assert(width_ >= 0.f && width_ <= 1.f);
		Assert(height_ >= 0.f && height_ <= 1.f);
		Assert(x_ + width_ <= 1.f);
		Assert(y_ + height_ <= 1.f);
	}
};

constexpr NormalizedRect Normalize(const Rect& rect, const Extents2D& bounds)
{
	return NormalizedRect(rect.x / bounds.width, rect.y / bounds.height, rect.width / bounds.width, rect.height / bounds.height);
}

constexpr Rect ToRect(const IntRect& rect)
{
	return Rect((float32)rect.x, (float32)rect.y,
		(float32)rect.width, (float32)rect.height);
}
