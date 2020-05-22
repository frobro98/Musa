// Copyright 2020, Nathan Blane

#include "MipmapGeneration.hpp"
#include "FastFourierTransform.hpp"
#include "Texture2D/Texture.h"
#include "Math/MathFunctions.hpp"
#include "BasicTypes/Color.hpp"
#include "Texture2D/TextureUtilities.hpp"

namespace
{
struct ImageDividedChannels
{
	DynamicArray<uint8> redChannel;
	DynamicArray<uint8> greenChannel;
	DynamicArray<uint8> blueChannel;
};

void SplitTextureIntoChannels(const MipmapLevel& texture, ImageDividedChannels& image)
{
	constexpr uint32 numChannels = 4;
	const uint32 rowSize = texture.width * numChannels;
	const uint8* mipData = texture.mipData.GetData();

	image.redChannel.Reserve(texture.width * texture.height);
	image.greenChannel.Reserve(texture.width * texture.height);
	image.blueChannel.Reserve(texture.width * texture.height);
	for (uint32 i = 0; i < texture.height; ++i)
	{
		for (uint32 j = 0; j < texture.width * numChannels; j += 4)
		{
			image.redChannel.Add(mipData[(j + rowSize * i) + 0]);
			image.greenChannel.Add(mipData[(j + rowSize * i) + 1]);
			image.blueChannel.Add(mipData[(j + rowSize * i) + 2]);
		}
	}
}

void CombineImageChannels(const ImageDividedChannels& image, MipmapLevel& level)
{
	constexpr uint32 numChannels = 4;
	const uint32 rowSize = level.width * numChannels;
	const uint32 mipLevelSize = rowSize * level.height;
	uint8* mipData = new uint8[mipLevelSize];
	ZeroMem(mipData, mipLevelSize);

	uint8* iter = mipData;
	for (uint32 i = 0; i < level.height; ++i)
	{
		for (uint32 j = 0; j < level.width; ++j)
		{
			*iter++ = (image.redChannel[j + level.width * i]);
			*iter++ = (image.greenChannel[j + level.width * i]);
			*iter++ = (image.blueChannel[j + level.width * i]);
			*iter++ = ((uint8)255);
		}
	}

	level.mipData = ResourceBlob(mipData, mipLevelSize);
}

constexpr uint32 GoodKaiserWidth = 32;

float bessel0Function(float x)
{
	const float EPSILON_RATIO = 1E-8f;
	float xh, sum, pow, ds;

	xh = .5f * x;
	sum = 1.f;
	pow = 1.f;
	ds = 1.f;

	int k = 0;
	while (ds > sum * EPSILON_RATIO)
	{
		k++;
		pow *= (xh / k);
		ds = pow * pow;
		sum = sum + ds;
	}

	return sum;
}

float kaiserFunction(float alpha, float halfWidth, float x)
{
	float ratio = (x / halfWidth);
	float kaiser = bessel0Function(alpha * Math::Sqrt(1 - ratio * ratio)) / bessel0Function(alpha);
	return kaiser;
}

template<uint32 N>
struct ImageFilter
{
	float data[N] = {};
};

template<uint32 N>
void NormalizeFilter(ImageFilter<N>& filter)
{
	float sum = 0.f;

	for (uint32 i = 0; i < N; ++i)
	{
		sum += filter.data[i];
	}

	Assert(sum != 0);

	for (uint32 i = 0; i < N; ++i)
	{
		filter.data[i] /= sum;
	}
}

ImageFilter<3> InitializeBoxFilter()
{
	ImageFilter<3> boxFilter = {};
	boxFilter.data[0] = 1;
	boxFilter.data[1] = 2;
	boxFilter.data[2] = 1;
	NormalizeFilter(boxFilter);

	return boxFilter;
}

ImageFilter<GoodKaiserWidth> InitializeKaiserFilter()
{
	ImageFilter<GoodKaiserWidth> kaiserFilter = {};
	const float halfWidth = GoodKaiserWidth * .5f;
	const float offset = -halfWidth;
	const float nudge = .5f;
	const float stretch = 1.f;
	const float alpha = 4.f;

	for (uint32 i = 0; i < GoodKaiserWidth; ++i)
	{
		const float x = (i + offset) + nudge;
		const float xStretchAdjust = x * stretch;
		const float sincValue = Math::Sinc(xStretchAdjust);
		const float windowValue = kaiserFunction(alpha, halfWidth, xStretchAdjust);

		kaiserFilter.data[i] = sincValue * windowValue;
	}

	NormalizeFilter(kaiserFilter);

	return kaiserFilter;
}

uint32 Get1DColorIndex(int32 i, int32 j, uint32 width, uint32 height)
{
	Assert(i >= 0 && i < ((int32)width));
	Assert(j >= 0 && j < ((int32)height));

	return static_cast<uint32>(i + j * width);
}

uint32 AdjustIndexforFiltering(int32 i, int32 j, uint32 width, uint32 height)
{
	while ((i < 0) || (i > ((int32)width - 1)))
	{
		if (i < 0) i = -i;
		if (i >= (int32)(width)) i = (int32)(width + width) - i - 1;
	}

	while ((j < 0) || (j > ((int32)height - 1)))
	{
		if (j < 0) j = -j;
		if (j >= (int32)height) j = (int32)(height + height) - j - 1;
	}

	return Get1DColorIndex(i, j, width, height);
}

// template<uint32 N>
// void ApplyFilterToLevel(const ImageFilter<N>& filter, const MipMapLevel& parent, MipMapLevel& level)

template<uint32 N>
void ApplyFilterToLevel(const ImageFilter<N>& filter,uint32 parentWidth, uint32 parentHeight, const DynamicArray<uint8>& channelData, DynamicArray<uint8>& processedChannelData)
{
	static_assert(N != 1, "Width of a filter must be usable");

	DynamicArray<float> normalizedPixelData = NormalizePixelData(channelData);
	DynamicArray<float> tmpData(normalizedPixelData.Size());
	int32 filterOffset = -static_cast<int32>((N / 2) - 1);

	uint32 levelWidth = parentWidth >> 1;
	uint32 levelHeight = parentHeight >> 1;
	DynamicArray<float> normalizedResult(levelWidth * levelHeight);

	for (int32 y = 0; y < (int32)parentHeight; ++y)
	{
		for (int32 x = 0; x < (int32)parentWidth; x += 2)
		{
			uint32 resultingIndex = Get1DColorIndex(x >> 1, y, parentWidth, parentHeight);
			float sum = 0;

			for (int32 k = 0; k < N; ++k)
			{
				int32 srcX = x + k + filterOffset;
				uint32 colorIndex = AdjustIndexforFiltering(srcX, y, parentWidth, parentHeight);
				const float color = normalizedPixelData[colorIndex];
				sum += filter.data[k] * color;
			}

			tmpData[resultingIndex] = sum;
		}
	}

	uint32 numColorsTouched = 0;
	for (int32 x = 0; x < (int32)(parentWidth >> 1); ++x)
	{
		for (int32 y = 0; y < (int32)parentHeight; y += 2)
		{
			uint32 resultingIndex = Get1DColorIndex(x, y >> 1, levelWidth, levelHeight);
			float sum = 0;

			for (int32 k = 0; k < N; ++k)
			{
				int32 srcY = y + k + filterOffset;
				uint32 colorIndex = AdjustIndexforFiltering(x, srcY, parentWidth, parentHeight);
				const float channelColor = tmpData[colorIndex];
				sum += filter.data[k] * channelColor;
			}

			normalizedResult[resultingIndex] = sum;
			++numColorsTouched;
		}
	}

	processedChannelData = UnnormalizePixelData(normalizedResult);
}

template<uint32 N>
MipmapLevel ProcessIndividualLevel(const ImageFilter<N>& filter, const MipmapLevel& parentLevel)
{
	MipmapLevel currentLevel;
	ImageDividedChannels parentImage;
	SplitTextureIntoChannels(parentLevel, parentImage);

	currentLevel.width = parentLevel.width >> 1;
	currentLevel.height = parentLevel.height >> 1;

	ImageDividedChannels processedImage;
	ApplyFilterToLevel(filter, parentLevel.width, parentLevel.height, parentImage.redChannel, processedImage.redChannel);
	ApplyFilterToLevel(filter, parentLevel.width, parentLevel.height, parentImage.greenChannel, processedImage.greenChannel);
	ApplyFilterToLevel(filter, parentLevel.width, parentLevel.height, parentImage.blueChannel, processedImage.blueChannel);

	CombineImageChannels(processedImage, currentLevel);

	return currentLevel;
}

void ApplyFFTToLevel(uint32 parentWidth, uint32 parentHeight, const DynamicArray<uint8>& channelData, DynamicArray<uint8>& processedChannel)
{
	DynamicArray<float> normalizedPixelData = NormalizePixelData(channelData);
	
	uint32 pixelDataSize = normalizedPixelData.Size();
	DynamicArray<float> tmpReal(pixelDataSize);
	DynamicArray<float> tmpImaginary(pixelDataSize);
	DynamicArray<float> freqReal(pixelDataSize);
	DynamicArray<float> freqImaginary(pixelDataSize);

	uint32 maxDimension = parentWidth;
	if (parentHeight > maxDimension)
	{
		maxDimension = parentHeight;
	}

	DynamicArray<float> inputReal(maxDimension);
	DynamicArray<float> outputReal(maxDimension);
	DynamicArray<float> inputImaginary(maxDimension);
	DynamicArray<float> outputImaginary(maxDimension);

	// Do the two-dimensional forward FFT.  To accomplish
	// this we first do a horizontal pass, then a vertical
	// pass.

	int32 parentWidthAsInt = static_cast<int32>(parentWidth);
	int32 parentHeightAsInt = static_cast<int32>(parentHeight);
	// Horizontal pass.
	for (int32 y = 0; y < parentHeightAsInt; ++y)
	{
		for (int32 x = 0; x < parentWidthAsInt; ++x)
		{
			uint32 src = static_cast<uint32>(x);
			uint32 index = Get1DColorIndex(x, y, parentWidth, parentHeight);
			inputReal[src] = normalizedPixelData[index];
			inputImaginary[src] = 0.f;
		}

		FastFourier(parentWidth, inputReal, inputImaginary, outputReal, outputImaginary);

		for (int32 x = 0; x < parentWidthAsInt; ++x)
		{
			uint32 dest = static_cast<uint32>(x);
			uint32 index = Get1DColorIndex(x, y, parentWidth, parentHeight);
			tmpReal[index] = outputReal[dest];
			tmpImaginary[index] = outputImaginary[dest];
		}
	}

	// Vertical Pass

	for (int32 x = 0; x < parentWidthAsInt; ++x)
	{
		for (int32 y = 0; y < parentHeightAsInt; ++y)
		{
			uint32 src = static_cast<uint32>(y);
			uint32 index = Get1DColorIndex(x, y, parentWidth, parentHeight);
			inputReal[src] = tmpReal[index];
			inputImaginary[src] = tmpImaginary[index];
		}

		FastFourier(parentHeight, inputReal, inputImaginary, outputReal, outputImaginary);

		for (int32 y = 0; y < parentHeightAsInt; ++y)
		{
			uint32 dest = static_cast<uint32>(y);
			uint32 index = Get1DColorIndex(x, y, parentWidth, parentHeight);
			freqReal[index] = outputReal[dest];
			freqImaginary[index] = outputImaginary[dest];
		}
	}

	int32 halfwidth = parentWidthAsInt / 2;
	int32 quarterwidth = parentWidthAsInt / 4;

	int32 halfheight = parentHeightAsInt / 2;
	int32 quarterheight = parentHeightAsInt / 4;

	int32 x0 = quarterwidth;
	int32 x1 = halfwidth + quarterwidth;
	int32 y0 = quarterheight;
	int32 y1 = halfheight + quarterheight;

	for (int32 y = 0; y < parentHeightAsInt; ++y)
	{
		for (int32 x = 0; x < parentWidthAsInt; ++x)
		{
			uint32 index = Get1DColorIndex(x, y, parentWidth, parentHeight);
			if ((x >= x0 && x < x1) || (y >= y0 && y < y1))
			{
				freqReal[index] = 0;
				freqImaginary[index] = 0;
			}
		}
	}

	// Reverse FFT.  For kicks we do vertical pass first,
	// then horizontal pass, though the order should not matter.

	// Vertical pass.
	for (int32 x = 0; x < parentWidthAsInt; ++x)
	{
		for (int32 y = 0; y < parentHeightAsInt; ++y)
		{
			uint32 src = static_cast<uint32>(y);
			uint32 index = Get1DColorIndex(x, y, parentWidth, parentHeight);
			inputReal[src] = freqReal[index];
			inputImaginary[src] = freqImaginary[index];
		}

		InverseFastFourier(parentHeight, inputReal, inputImaginary, outputReal, outputImaginary);

		for (int32 y = 0; y < parentHeightAsInt; ++y)
		{
			uint32 dest = static_cast<uint32>(y);
			uint32 index = Get1DColorIndex(x, y, parentWidth, parentHeight);
			tmpReal[index] = outputReal[dest];
			tmpImaginary[index] = outputImaginary[dest];
		}
	}

	// Horizontal Pass
	for (int32 y = 0; y < parentHeightAsInt; ++y)
	{
		for (int32 x = 0; x < parentWidthAsInt; ++x)
		{
			uint32 src = static_cast<uint32>(x);
			uint32 index = Get1DColorIndex(x, y, parentWidth, parentHeight);
			inputReal[src] = tmpReal[index];
			inputImaginary[src] = tmpImaginary[index];
		}

		InverseFastFourier(parentWidth, inputReal, inputImaginary, outputReal, outputImaginary);

		for (int32 x = 0; x < parentWidthAsInt; ++x)
		{
			uint32 dest = static_cast<uint32>(x);
			uint32 index = Get1DColorIndex(x, y, parentWidth, parentHeight);
			tmpReal[index] = outputReal[dest];
			// outputImaginary is discarded.  Should be 0 within roundoff error.
		}
	}

	uint32 levelWidth = parentWidth >> 1;
	uint32 levelHeight = parentHeight >> 1;
	DynamicArray<float> normalizedResult(levelWidth * levelHeight);

	for (int32 y = 0; y < parentHeightAsInt; y += 2)
	{
		for (int32 x = 0; x < parentWidthAsInt; ++x)
		{
			uint32 srcIndex = Get1DColorIndex(x, y, parentWidth, parentHeight);
			uint32 destIndex = Get1DColorIndex(x >> 1, y >> 1, levelWidth, levelHeight);
			normalizedResult[destIndex] = tmpReal[srcIndex];
		}
	}

	processedChannel = UnnormalizePixelData(normalizedResult);

}

MipmapLevel ProcessIndividualLevelWithFFT(const MipmapLevel& parentLevel)
{
	MipmapLevel currentLevel;
	currentLevel.width = parentLevel.width >> 1;
	currentLevel.height = parentLevel.height >> 1;

	ImageDividedChannels parentImage;
	SplitTextureIntoChannels(parentLevel, parentImage);

	ImageDividedChannels processedImage;
	ApplyFFTToLevel(parentLevel.width, parentLevel.height, parentImage.redChannel, processedImage.redChannel);
	ApplyFFTToLevel(parentLevel.width, parentLevel.height, parentImage.greenChannel, processedImage.greenChannel);
	ApplyFFTToLevel(parentLevel.width, parentLevel.height, parentImage.blueChannel, processedImage.blueChannel);

	CombineImageChannels(processedImage, currentLevel);

	return currentLevel;
}

template<uint32 N>
void ProcessMipLevels(const ImageFilter<N>& filter, DynamicArray<MipmapLevel>& mipMapLevels, uint32 levelsToGen)
{
	const MipmapLevel* parentLevel = &mipMapLevels[0];
	for (uint32 i = 1; i < levelsToGen; ++i)
	{
		mipMapLevels.Add(ProcessIndividualLevel(filter, *parentLevel));
		parentLevel = &mipMapLevels[i];
	}
}

static auto boxFilter = InitializeBoxFilter();
static auto kaiserFilter = InitializeKaiserFilter();

void ProcessMipLevelsWithBoxFilter(DynamicArray<MipmapLevel>& mipMapLevels, uint32 levelsToGen)
{
	const auto& filter = boxFilter;
	ProcessMipLevels(filter, mipMapLevels, levelsToGen);
}

void ProcessMipLevelsWithKaiserFilter(DynamicArray<MipmapLevel>& mipMapLevels, uint32 levelsToGen)
{
	const auto& filter = kaiserFilter;
	ProcessMipLevels(filter, mipMapLevels, levelsToGen);
}

void ProcessMipLevelsWithFFT(DynamicArray<MipmapLevel>& mipMapLevels, uint32 levelsToGen)
{
	const MipmapLevel* parentLevel = &mipMapLevels[0];
	for (uint32 i = 1; i < levelsToGen; ++i)
	{
		mipMapLevels.Add(ProcessIndividualLevelWithFFT(*parentLevel));
		parentLevel = &mipMapLevels[i];
	}
}

}

uint32 GetMaxMipMapLevels(uint32 width, uint32 height)
{
	float maxDimention = static_cast<float>(Math::Max(width, height));
	uint32 numPossibleLevels = 1 + static_cast<uint32>(Math::Floor(Math::Log2(maxDimention)));
	return numPossibleLevels;
}

bool GenerateMipMapLevels(Texture& texture, uint32 levelsToGen, MipMapFilter filter)
{
	Assert(texture.mipLevels.Size() > 0);
	uint32 width = texture.GetWidth();
	uint32 height = texture.GetHeight();
	Assert(levelsToGen < GetMaxMipMapLevels(width, height));

	switch (filter)
	{
		case MipMapFilter::Box:	
		{
			ProcessMipLevelsWithBoxFilter(texture.mipLevels, levelsToGen);
		}break;
		case MipMapFilter::Kaiser:
		{
			ProcessMipLevelsWithKaiserFilter(texture.mipLevels, levelsToGen);
		}break;
		case MipMapFilter::FFT:
		{
			ProcessMipLevelsWithFFT(texture.mipLevels, levelsToGen);
		}break;
	}

	return true;
}

/*
// BOX FILTERING?
MipMapLevel currentLevel;
uint32 parentWidth = parentLevel->width;
uint32 parentHeight = parentLevel->height;
width = parentWidth >> 1;
height = parentHeight >> 1;

for (uint32 y = 0; y < height * numColorComponents; y += numColorComponents)
{
	for (uint32 x = 0; x < width * numColorComponents; x += numColorComponents)
	{
		const Color* topLeft = reinterpret_cast<const Color*>(&parentLevel->imageData[x + parentWidth * y]);
		const Color* topRight = reinterpret_cast<const Color*>(&parentLevel->imageData[(x + numColorComponents) + parentWidth * y]);
		const Color* botLeft = reinterpret_cast<const Color*>(&parentLevel->imageData[x + parentWidth * (y + 1)]);
		const Color* botRight = reinterpret_cast<const Color*>(&parentLevel->imageData[(x + numColorComponents) + parentWidth * (y + 1)]);

		int32 r = topLeft->r + topRight->r + botLeft->r + botRight->r;
		r /= 4;
		int32 g = topLeft->g + topRight->g + botLeft->g + botRight->g;
		g /= 4;
		int32 b = topLeft->b + topRight->b + botLeft->b + botRight->b;
		b /= 4;
		int32 a = topLeft->a + topRight->a + botLeft->a + botRight->a;
		a /= 4;

		currentLevel.imageData.Add(static_cast<uint8>(r));
		currentLevel.imageData.Add(static_cast<uint8>(g));
		currentLevel.imageData.Add(static_cast<uint8>(b));
		currentLevel.imageData.Add(static_cast<uint8>(a));
	}
}
//*/