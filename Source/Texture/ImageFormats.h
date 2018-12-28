#pragma once

enum class ImageFormat
{
	RGB_8,
	BGR_8,
	RGBA_8,
	BGRA_8,
	Gray_8,
	BC1,
	BC3,
	BC7,
	Invalid
};

enum class CompressionFormat
{
	Invalid,
	BC1,
	BC3,
	BC7
};

constexpr const char* CompressionFormatString(CompressionFormat format)
{
	switch (format)
	{
		case CompressionFormat::BC1:
			return "BC1";
		case CompressionFormat::BC3:
			return "BC3";
		case CompressionFormat::BC7:
			return "BC7";
		case CompressionFormat::Invalid:
		default:
			return "Invalid";
	}
}