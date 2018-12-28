
#include "TextureProcessing.hpp"
#include "Containers/Map.h"
#include "File/Path.hpp"
#include "File/FileUtilities.hpp"
#include "Importers/JPEGImporter.hpp"
#include "Importers/TGAImporter.h"
#include "Importers/PNGImporter.hpp"
#include "Importers/BMPImporter.hpp"
#include "Importers/TextureCompression.h"
#include "Math/MathUtilities.h"
#include "Color.hpp"

namespace
{
constexpr const char* supportedImageTypes[] =
{
	"jpg",
	"tga",
	"png",
	"bmp"
};

Map<String, TextureImporter*> importerMap = {
	{"jpg", new JPEGImporter},
	{"tga", new TGAImporter},
	{"png", new PNGImporter},
	{"bmp", new BMPImporter}
};

Texture ConstructTexture(TextureImporter& importer)
{
	Texture tex;
	MipMapLevel baseLevel = {};
	baseLevel.width = importer.GetWidth();
	baseLevel.height = importer.GetHeight();
	baseLevel.imageData = importer.GetImportedPixelData();
	tex.mipLevels.Add(std::move(baseLevel));
	tex.format = importer.GetFormat();
	return tex;
}

}

bool IsSupportedTexture(const Path& texturePath)
{
	if (texturePath.DoesFileExist())
	{
		String extension = texturePath.GetFileExtension();
		for (auto* imageType : supportedImageTypes)
		{
			if (extension == imageType)
			{
				return true;
			}
		}
	}

	return false;
}

Texture ProcessImageFile(const Path& filePath, CompressionFormat format)
{
	Assert(filePath.DoesFileExist());

	Array<uint8> textureData = LoadFileToMemory(filePath);
	String extension = filePath.GetFileExtension();
	
	TextureImporter* importer = importerMap[*extension];
	importer->SetImportData(textureData);
	if (importer->IsValid())
	{
		importer->Import();
	}

	String textureName = filePath.GetFileNameWithoutExtension();
	Texture originalTexture = ConstructTexture(*importer);
	Texture compressedTexture;
	if (format != CompressionFormat::Invalid)
	{
		Compression::CompressForGPU(originalTexture, compressedTexture, format);
		compressedTexture.name = std::move(textureName);
		return compressedTexture;
	}

	originalTexture.name = std::move(textureName);
	return originalTexture;
}
