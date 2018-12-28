#pragma once

#include "Types.h"
#include "ImageFormats.h"
#include "Containers/Array.h"
#include "Math/MathUtilities.h"

constexpr uint32 FormatInBytes(ImageFormat format)
{
	switch (format)
	{
		case ImageFormat::RGB_8:
			return 3;
		case ImageFormat::BGR_8:
			return 3;
		case ImageFormat::RGBA_8:
			return 4;
		case ImageFormat::BGRA_8:
			return 4;
		case ImageFormat::Gray_8:
			return 1;
		case ImageFormat::BC1:
			return 8;
		case ImageFormat::BC3:
		case ImageFormat::BC7:
			return 16;
		case ImageFormat::Invalid:
			break;
	}
	return 0;
}

inline Array<float> NormalizePixelData(const Array<uint8>& pixelData)
{
	Array<float> normalizedPixelData(pixelData.Size());

	for (uint32 i = 0; i < pixelData.Size(); ++i)
	{
		normalizedPixelData[i] = pixelData[i] / 255.f;
	}

	return normalizedPixelData;
}

inline Array<uint8> UnnormalizePixelData(const Array<float>& normalizedPixelData)
{
	Array<uint8> unnormalizedData(normalizedPixelData.Size());

	for (uint32 i = 0; i < normalizedPixelData.Size(); ++i)
	{
		float clampedVal = Math::Clamp((normalizedPixelData[i] * 255.f), 0.f, 255.f);
		unnormalizedData[i] = static_cast<uint8>(clampedVal);
// 		if (unnormalizedData[i] == 0)
// 		{
// 			printf("%u ", i);
// 		}
	}

	return unnormalizedData;
}
