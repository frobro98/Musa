#include "TextureImporter.hpp"
#include "File/FileUtilities.hpp"

uint32 TextureImporter::GetWidth() const
{
	return static_cast<uint32>(width);
}

uint32 TextureImporter::GetHeight() const
{
	return static_cast<uint32>(height);
}

ImageFormat TextureImporter::GetFormat() const
{
	return format;
}

bool TextureImporter::IsValid() const
{
	return validData;
}

void TextureImporter::Import()
{
	ProcessImport();
}

const DynamicArray<uint8>& TextureImporter::GetImportedPixelData() const
{
	return importedImageData;
}
