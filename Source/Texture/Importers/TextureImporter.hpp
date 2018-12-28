#pragma once

#include "Containers/Array.h"
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

	virtual void SetImportData(const Array<uint8>& compressedData) = 0;

	bool IsValid() const;
	void Import();
	const Array<uint8>& GetImportedPixelData() const;

protected:
	virtual void ProcessImport() = 0;

protected:
	Array<uint8> importData;
	Array<uint8> importedImageData;
	ImageFormat format = ImageFormat::Invalid;
	int32 width;
	int32 height;
	bool validData;
	bool pad[3];
};