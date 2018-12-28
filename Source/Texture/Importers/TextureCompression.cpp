
#include "Platform.h"
#include "Assertion.h"
#include "ImageFormats.h"
#include "TextureCompression.h"
#include "MemoryUtilities.h"
#include "Texture2D/TextureUtilities.hpp"
WALL_WRN_PUSH
#include "ispc/ispc_texcomp.h"
WALL_WRN_POP

namespace Compression
{

namespace
{
constexpr uint32 CompressonDenom = 4;
constexpr uint32 ChannelsPerPixel = 4;

void ForceMipDimensionForCompression(MipMapLevel& level)
{
	uint32 origWidth = level.width;
	uint32 origHeight = level.height;
	uint32 adjustedWidth = origWidth % CompressonDenom == 0 ? origWidth : ((origWidth + 3) / CompressonDenom);
	uint32 adjustedHeight = origHeight % CompressonDenom == 0 ? origHeight : ((origHeight + 3) / CompressonDenom);

	if (adjustedWidth > origWidth || adjustedHeight > origHeight)
	{

		Array<uint8> adjustedData(adjustedWidth * adjustedHeight * ChannelsPerPixel);
		const uint8* src = level.imageData.GetData();
		uint8* dst = adjustedData.GetData();
		const uint32 origRowWidth = origWidth * ChannelsPerPixel;
		const uint32 adjustedRowWidth = adjustedWidth * ChannelsPerPixel;
		for (uint32 i = 0; i < origHeight; ++i)
		{
			// Copy row of pixels then move to next row
			Memcpy(dst, adjustedRowWidth, src, origRowWidth);
			src += origRowWidth;
			dst += adjustedRowWidth;
		}

		level.imageData = std::move(adjustedData);
	}
}

void ForceBufferDimensionForCompression(Texture& outDescription)
{
	for (auto& level : outDescription.mipLevels)
	{
		ForceMipDimensionForCompression(level);
	}

}

rgba_surface FormatMipForISPC(MipMapLevel& level)
{
	rgba_surface surface = {};
	surface.ptr = level.imageData.GetData();
	surface.width = static_cast<int32>(level.width);
	surface.height = static_cast<int32>(level.height);
	surface.stride = static_cast<int32>(level.width * ChannelsPerPixel);
	return surface;
}

void PrepareMipsForCompression(ImageFormat currentFormat, Array<MipMapLevel>& mipLevels)
{
	if (currentFormat == ImageFormat::BGRA_8 || currentFormat == ImageFormat::BGR_8)
	{
		for (auto& level : mipLevels)
		{
			const uint32 bytesPerPixel = FormatInBytes(currentFormat);
			for (uint32 i = 0; i < level.imageData.Size(); i += bytesPerPixel)
			{
				uint8 tmp = level.imageData[i + 0];
				level.imageData[i + 0] = level.imageData[i + 2];
				level.imageData[i + 2] = tmp;
			}
		}
	}
}

void PerformCompression(Texture& texture, CompressionFormat compressionFormat)
{
	for (auto& level : texture.mipLevels)
	{
		rgba_surface surface = FormatMipForISPC(level);
		Array<uint8> compressedData;
		uint32 numBlocksX = level.width > 4 ? level.width / 4 : 1;
		uint32 numBlocksY = level.height > 4 ? level.height / 4 : 1;
		switch (compressionFormat)
		{
			case CompressionFormat::BC1:
			{
				texture.format = ImageFormat::BC1;
				compressedData = Array<uint8>(numBlocksX * numBlocksY * 8);
				CompressBlocksBC1(&surface, compressedData.GetData());
			}break;
			case CompressionFormat::BC3:
			{
				texture.format = ImageFormat::BC3;
				compressedData = Array<uint8>(numBlocksX * numBlocksY * 16);
				CompressBlocksBC3(&surface, compressedData.GetData());
			}break;
			case CompressionFormat::BC7:
			{
				texture.format = ImageFormat::BC7;
				compressedData = Array<uint8>(numBlocksX * numBlocksY * 16);
				bc7_enc_settings settings;
				GetProfile_slow(&settings);
				CompressBlocksBC7(&surface, compressedData.GetData(), &settings);
			}break;
			case CompressionFormat::Invalid:
			default:
			{
				Assert(false);
			}
		}

		level.imageData = compressedData;
	}
}
}

void CompressForGPU(const Texture& inTexture, Texture& outTexture, CompressionFormat compressionFormat)
{
	outTexture.mipLevels = inTexture.mipLevels;

	ForceBufferDimensionForCompression(outTexture);
	PrepareMipsForCompression(inTexture.format, outTexture.mipLevels);
	PerformCompression(outTexture, compressionFormat);
}

}