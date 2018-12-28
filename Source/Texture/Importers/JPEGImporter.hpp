#pragma once

#include "Containers/Array.h"
#include "Importers/TextureImporter.hpp"
#include "ImageFormats.h"

class JPEGImporter : public TextureImporter
{
public:
	JPEGImporter();

// 	JPEGImporter(const JPEGImporter&) = delete;
// 	JPEGImporter(JPEGImporter&&) = delete;
// 	JPEGImporter& operator=(const JPEGImporter&) = delete;

	virtual void SetImportData(const Array<uint8>& compressedData) override;

private:
	virtual void ProcessImport() override;

private:
	int32 numComponents;
};