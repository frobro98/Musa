#pragma once

#include "Containers/DynamicArray.hpp"
#include "File/Path.hpp"
#include "ImageFormats.h"

// Consider making this a suite of functions that load specific kinds of image files
class TextureImporter
{
public:
	TextureImporter() = default;
	virtual ~TextureImporter() = default;

	uint32 GetWidth() const;
	uint32 GetHeight() const;
	ImageFormat GetFormat() const;

	virtual void SetImportData(const DynamicArray<uint8>& compressedData) = 0;

	bool IsValid() const;
	void Import();
	const DynamicArray<uint8>& GetImportedPixelData() const;

protected:
	virtual void ProcessImport() = 0;

protected:
	DynamicArray<uint8> importData;
	DynamicArray<uint8> importedImageData;
	ImageFormat format = ImageFormat::Invalid;
	int32 width = 0;
	int32 height = 0;
	bool validData = false;
};