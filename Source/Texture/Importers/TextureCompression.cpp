
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

void ForceMipDimensionForCompression(MipmapLevel& level)
{
	uint32 origWidth = level.width;
	uint32 origHeight = level.height;
	uint32 adjustedWidth = origWidth % CompressonDenom == 0 ? origWidth : ((origWidth + 3) / CompressonDenom);
	uint32 adjustedHeight = origHeight % CompressonDenom == 0 ? origHeight : ((origHeight + 3) / CompressonDenom);

	if (adjustedWidth > origWidth || adjustedHeight > origHeight)
	{
		uint32 dataSize = adjustedWidth * adjustedHeight * ChannelsPerPixel;
		uint8* adjustedData = new uint8[dataSize];
		const uint8* src = level.mipData.GetData();
		uint8* dst = adjustedData;
		const uint32 origRowWidth = origWidth * ChannelsPerPixel;
		const uint32 adjustedRowWidth = adjustedWidth * ChannelsPerPixel;
		for (uint32 i = 0; i < origHeight; ++i)
		{
			// Copy row of pixels then move to next row
			Memcpy(dst, adjustedRowWidth, src, origRowWidth);
			src += origRowWidth;
			dst += adjustedRowWidth;
		}

		level.mipData = ResourceBlob(adjustedData, dataSize);
	}
}

void ForceBufferDimensionForCompression(Texture& outDescription)
{
	for (auto& level : outDescription.mipLevels)
	{
		ForceMipDimensionForCompression(level);
	}

}

rgba_surface FormatMipForISPC(MipmapLevel& level)
{
	rgba_surface surface = {};
	surface.ptr = level.mipData.GetData();
	surface.width = static_cast<int32>(level.width);
	surface.height = static_cast<int32>(level.height);
	surface.stride = static_cast<int32>(level.width * ChannelsPerPixel);
	return surface;
}

void PrepareMipsForCompression(ImageFormat currentFormat, DynamicArray<MipmapLevel>& mipLevels)
{
	if (currentFormat == ImageFormat::BGRA_8norm || currentFormat == ImageFormat::BGR_8u)
	{
		for (auto& level : mipLevels)
		{
			const uint32 bytesPerPixel = FormatInBytes(currentFormat);
			uint8* imageData = level.mipData.GetData();
			for (uint32 i = 0; i < level.mipData.GetSize(); i += bytesPerPixel)
			{
				uint8 tmp = imageData[i + 0];
				imageData[i + 0] = imageData[i + 2];
				imageData[i + 2] = tmp;
			}
		}
	}
}

void PerformCompression(Texture& texture, CompressionFormat compressionFormat)
{
	for (auto& level : texture.mipLevels)
	{
		rgba_surface surface = FormatMipForISPC(level);
		uint8* compressedData;
		uint32 compressedSize;
		uint32 numBlocksX = level.width > 4 ? level.width / 4 : 1;
		uint32 numBlocksY = level.height > 4 ? level.height / 4 : 1;
		switch (compressionFormat)
		{
			case CompressionFormat::BC1:
			{
				texture.format = ImageFormat::BC1;
				compressedSize = numBlocksX * numBlocksY * 8;
				compressedData = new uint8[compressedSize];
				CompressBlocksBC1(&surface, compressedData);
			}break;
			case CompressionFormat::BC3:
			{
				texture.format = ImageFormat::BC3;
				compressedSize = numBlocksX * numBlocksY * 16;
				compressedData = new uint8[compressedSize];
				CompressBlocksBC3(&surface, compressedData);
			}break;
			case CompressionFormat::BC7:
			{
				texture.format = ImageFormat::BC7;
				compressedSize = numBlocksX * numBlocksY * 16;
				compressedData = new uint8[compressedSize];
				bc7_enc_settings settings;
				GetProfile_slow(&settings);
				CompressBlocksBC7(&surface, compressedData, &settings);
			}break;
			case CompressionFormat::Invalid:
			default:
			{
				compressedData = nullptr;
				compressedSize = 0;
				Assert(false);
			}
		}

		level.mipData = ResourceBlob(compressedData, compressedSize);
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