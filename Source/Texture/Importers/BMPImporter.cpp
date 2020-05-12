// Copyright 2020, Nathan Blane

#include "BMPImporter.hpp"
#include "Math/MathFunctions.hpp"

namespace
{
enum class BMPCompressionMethod
{
	BMP_RGB = 0,

	// Unsupported bmp methods
// 	BMP_RLE8,
// 	BMP_RLE4,
// 	BMP_BITFIELDS,
// 	BMP_JPEG,
// 	BMP_PNG,
// 	BMP_ALPHABITFIELDS,
// 	BMP_CMYK,
// 	BMP_CMYKRLE8,
// 	BMP_CMYKRLE4
};

#pragma pack(push, 1)
struct BMPFileHeader
{
	uint16 bmpHeaderField;
	uint32 bmpFileSize;
	uint16 reserved[2]; // 2 reserved fields
	uint32 imageDataOffset;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct BMPInfoHeader
{
	uint32 headerSize;
	int32  bmpWidth;
	int32  bmpHeight;
	uint16 colorPlaneCount = 1;
	uint16 bmpBitDepth;
	BMPCompressionMethod compression;
	uint32 bmpImageSize;
	int32  resolutionX; // pixels per meter
	int32  resolutionY; // pixels per meter
	uint32 numColorsInPalette;
	uint32 numImportantColors;
};
#pragma pack(pop)

bool BMPFileValid(uint16 fileCode)
{
	const uint8* fileCodeBytes = reinterpret_cast<uint8*>(&fileCode);
	return fileCodeBytes[0] == 'B' && fileCodeBytes[1] == 'M';
}
}

void BMPImporter::SetImportData(const DynamicArray<uint8>& dataForImport)
{
	importData = dataForImport;
	ProcessBMPHeader();
}

void BMPImporter::ProcessImport()
{
	// Negative height if 0, 0 of the image is in the top left corner
	const bool isHeightNegative = height < 0;
	height = Math::Abs(height);

	const int32 bytesPerRowAligned = Align(width * channels, 4);
	const uint8* pixelData = importData.GetData() + pixelDataOffset;
	int32 rowAdvance = -bytesPerRowAligned;
	if (isHeightNegative)
	{
		rowAdvance *= -1;
	}
	else
	{
		pixelData += (uintptr_t)(bytesPerRowAligned * (height - 1));
	}
	// Get at the pixel data
	uint8* importedPixels = importedImageData.GetData();
	if (channels == 3)
	{
		for (uint32 i = 0; i < static_cast<uint32>(height); ++i)
		{
			const uint8* rowData = pixelData;
			for (uint32 j = 0; j < static_cast<uint32>(width); ++j)
			{
				*importedPixels = *rowData;
				++importedPixels;
				++rowData;

				*importedPixels = *rowData;
				++importedPixels;
				++rowData;

				*importedPixels = *rowData;
				++importedPixels;
				++rowData;
				
				*importedPixels = 0xff;
				++importedPixels;

			}
			pixelData += rowAdvance; // If origin is at bottom left corner, modifier is negative
		}
	}
	else if (channels == 4)
	{
		for (uint32 i = 0; i < static_cast<uint32>(height); ++i)
		{
			const uint8* rowData = pixelData;
			for (uint32 j = 0; j < static_cast<uint32>(width); ++j)
			{
				*importedPixels = *rowData;
				++importedPixels;
				++rowData;

				*importedPixels = *rowData;
				++importedPixels;
				++rowData;

				*importedPixels = *rowData;
				++importedPixels;
				++rowData;

				*importedPixels = *rowData;
				++importedPixels;
				++rowData;
			}
			pixelData += rowAdvance; // If origin is at bottom left corner, modifier is negative
		}
	}
	else
	{
		Assert(false);
	}
}

void BMPImporter::ProcessBMPHeader()
{
	const BMPFileHeader* header = reinterpret_cast<BMPFileHeader*>(importData.GetData());
	validData = BMPFileValid(header->bmpHeaderField);
	if (validData)
	{
		const BMPInfoHeader* bmpInfo = reinterpret_cast<BMPInfoHeader*>(importData.GetData() + sizeof(BMPFileHeader));
		pixelDataOffset = header->imageDataOffset;

		Assert(bmpInfo->compression == BMPCompressionMethod::BMP_RGB);

		width = bmpInfo->bmpWidth;
		height = bmpInfo->bmpHeight;
		bitDepth = bmpInfo->bmpBitDepth;
		channels = bitDepth / 8;
		Assert(channels > 1);
		format = ImageFormat::BGRA_8norm;
		// Storing data as if it has 4 channels
		importedImageData.Resize(width * height * 4u);
	}
}
