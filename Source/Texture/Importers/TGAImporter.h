#pragma once

#include "PlatformDefinitions.h"
#include "Texture/ImageFormats.h"
#include "Importers/TextureImporter.hpp"

#pragma pack(push, 1)
struct TGAFileHeader
{
	int8 idSize;			// Size of ID field that folows header
	int8 colorMapType;		// 0 = none, 1 = paletted
	int8 imageType;			// 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8 = rle
	uint16 colorMapStart;	// First color map entry
	uint16 colorMapLength;	// Number of colors
	uint8 colorMapBits;		// bits per palette entry
	uint16 xStart;			// image x origin
	uint16 yStart;			// image y origin
	uint16 width;			// width in pixels
	uint16 height;			// height in pixels
	int8 bits;				// bits per pixel (8, 16, 24, 32)
	int8 descriptor;		// image descriptor
};
#pragma pack(pop)

class TGAImporter : public TextureImporter
{
public:
	TGAImporter();

	virtual void SetImportData(const Array<uint8>& compressedData) override;

private:
	virtual void ProcessImport() override;
	void ProcessHeader(const Array<uint8>& compressedData);
	void ModifyBGRImage();

private:
	TGAFileHeader tgaFile;
	uint8 pad[2];
	uint32 bitDepth;
};