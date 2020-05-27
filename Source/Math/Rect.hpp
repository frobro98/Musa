// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "Debugging/Assertion.hpp"
#include "BasicTypes/Extents.hpp"

struct IntRect;

struct Rect
{
	f32 x = 0;
	f32 y = 0;
	f32 width = 0;
	f32 height = 0;

	constexpr Rect() = default;

	constexpr Rect(f32 x_, f32 y_, f32 width_, f32 height_)
		: x(x_), y(y_), width(width_), height(height_)
	{
	}
};

struct IntRect
{
	i32 x = 0;
	i32 y = 0;
	i32 width = 0;
	i32 height = 0;

	constexpr IntRect() = default;

	constexpr IntRect(i32 x_, i32 y_, i32 width_, i32 height_)
		: x(x_), y(y_), width(width_), height(height_)
	{
	}
};

struct NormalizedRect
{

	f32 x = 0;
	f32 y = 0;
	f32 width = 0;
	f32 height = 0;

	constexpr NormalizedRect() = default;

	constexpr NormalizedRect(f32 x_, f32 y_, f32 width_, f32 height_)
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
	return Rect((f32)rect.x, (f32)rect.y,
		(f32)rect.width, (f32)rect.height);
}
