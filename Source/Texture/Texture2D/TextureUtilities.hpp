#pragma once

#include "Types/Intrinsics.hpp"
#include "ImageFormats.h"
#include "Containers/DynamicArray.hpp"
#include "Math/MathFunctions.hpp"

constexpr uint32 FormatInBytes(ImageFormat format)
{
	switch (format)
	{
		case ImageFormat::RGB_8u:
		case ImageFormat::RGB_8norm:
			return 3;
		case ImageFormat::BGR_8u:
			return 3;
		case ImageFormat::RGBA_8u:
		case ImageFormat::RGBA_8norm:
			return 4;
		case ImageFormat::BGRA_8norm:
			return 4;
		case ImageFormat::Gray_8u:
			return 1;
		case ImageFormat::BC1:
			return 8;
		case ImageFormat::BC3:
		case ImageFormat::BC7:
			return 16;
		case ImageFormat::D_32f:
			return 4;
		case ImageFormat::DS_32f_8u:
			return 5;
		case ImageFormat::DS_24f_8u:
			return 4;
		case ImageFormat::RGB_16f:
			return 6;
		case ImageFormat::RGBA_16f:
			return 8;
		case ImageFormat::Invalid:
			break;
	}
	return 0;
}

inline DynamicArray<float> NormalizePixelData(const DynamicArray<uint8>& pixelData)
{
	DynamicArray<float> normalizedPixelData(pixelData.Size());

	for (uint32 i = 0; i < pixelData.Size(); ++i)
	{
		normalizedPixelData[i] = pixelData[i] / 255.f;
	}

	return normalizedPixelData;
}

inline DynamicArray<uint8> UnnormalizePixelData(const DynamicArray<float>& normalizedPixelData)
{
	DynamicArray<uint8> unnormalizedData(normalizedPixelData.Size());

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
