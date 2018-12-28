
#include "Importers/TGAImporter.h"
#include "Texture2D/Texture.h"

#include "MemoryUtilities.h"

// TODO - Need to move these importer files to either a library project or somewhere more common...

TGAImporter::TGAImporter()
	: TextureImporter()
{
}

void TGAImporter::SetImportData(const Array<uint8>& data)
{
	ProcessHeader(data);
	importData = Array<uint8>(data.GetData() + sizeof(tgaFile), data.Size() - sizeof(tgaFile));
}

void TGAImporter::ProcessHeader(const Array<uint8>& data)
{
	Memcpy(&tgaFile, sizeof(TGAFileHeader), data.GetData(), sizeof(TGAFileHeader));
	validData = tgaFile.bits == 24 || tgaFile.bits == 32;

	width = tgaFile.width;
	height = tgaFile.height;
	bitDepth = tgaFile.bits / 8u;
	if (validData)
	{
		format = bitDepth == 3 ? ImageFormat::BGR_8 : ImageFormat::BGRA_8;
	}
	else if (tgaFile.bits == 8)
	{
		//  *eFormat = GL_LUMINANCE;
		//  *iComponents = GL_LUMINANCE;
		Assert(false);
	}
	else
	{
		// If on the iPhone, TGA's are BGR, and the iPhone does not 
		// support BGR without alpha, but it does support RGB,
		// so a simple swizzle of the red and blue bytes will suffice.
		// For faster iPhone loads however, save your TGA's with an Alpha!
	}
}

void TGAImporter::ModifyBGRImage()
{
	uint32 imageSize = width * height * bitDepth;
	uint8* texDataPtr = importedImageData.GetData();
	// Could be uint32 to allow always having an alpha
	for (uint32 i = 0; i < imageSize; ++i)
	{
		// Blue
		*texDataPtr = importData[i];
		++i, ++texDataPtr;

		// Green
		*texDataPtr = importData[i];
		++i, ++texDataPtr;

		// Red
		*texDataPtr = importData[i];
		++texDataPtr;

		// Alpha
		*texDataPtr = 0xff;
		++texDataPtr;
	}
	format = ImageFormat::BGRA_8;
}

void TGAImporter::ProcessImport()
{
	constexpr uint32 correctBitDepth = 4;
	const uint32 textureSize = width * height * correctBitDepth;
	importedImageData.Resize(textureSize);
	// Vulkan doesn't seem to support 24 bit textures, so here we are
	if (bitDepth == 3 && tgaFile.imageType < 8) // BGR no alpha
	{
		ModifyBGRImage();
	}
	else if (tgaFile.imageType < 8)
	{
		Memcpy(importedImageData.GetData(), textureSize, importData.GetData(), importData.Size());
	}
	else // RLE compression not currently supported!!
	{
		// TODO - implement RLE compression for tgas
		Assert(false);
	}
}
