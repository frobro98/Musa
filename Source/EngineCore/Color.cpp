#include "Color.hpp"

Color32 NormalizeColor(const Color& color)
{
	return Color32(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
}

Color UnnormalizeColor(const Color32& color)
{
	uint8 r = static_cast<uint8>(color.r * 255);
	uint8 g = static_cast<uint8>(color.g * 255);
	uint8 b = static_cast<uint8>(color.b * 255);
	uint8 a = static_cast<uint8>(color.a * 255);
	return Color(r, g, b, a);
}
