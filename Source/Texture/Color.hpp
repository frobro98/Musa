#pragma once

#include "EngineCore/Types.h"

struct Color
{
	constexpr Color() = default;
	constexpr Color(uint8 red, uint8 green, uint8 blue)
		: r(red),
		g(green),
		b(blue)
	{
	}
	constexpr Color(uint8 red, uint8 green, uint8 blue, uint8 alpha)
		: r(red),
		g(green),
		b(blue),
		a(alpha)
	{
	}

	uint8 r = 255;
	uint8 g = 255;
	uint8 b = 255;
	uint8 a = 255;

	static constexpr Color White()
	{
		return Color();
	}
	static constexpr Color Black()
	{
		return Color(0, 0, 0);
	}
	static constexpr Color Red()
	{
		return Color(255, 0, 0);
	}
	static constexpr Color Green()
	{
		return Color(0, 255, 0);
	}
	static constexpr Color Blue()
	{
		return Color(0, 0, 255);
	}
	static constexpr Color Magenta()
	{
		return Color(255, 0, 255);
	}
	static constexpr Color Cyan()
	{
		return Color(0, 255, 255);
	}
	static constexpr Color Yellow()
	{
		return Color(255, 255, 0);
	}
};

struct Color32
{
	constexpr Color32() = default;
	constexpr Color32(float red, float green, float blue)
		: r(red),
		g(green),
		b(blue)
	{
	}
	constexpr Color32(float red, float green, float blue, float alpha)
		: r(red),
		g(green),
		b(blue),
		a(alpha)
	{
	}

	float r = 1.f;
	float g = 1.f;
	float b = 1.f;
	float a = 1.f;
};

Color32 NormalizeColor(const Color& color);
Color	UnnormalizeColor(const Color32& color);