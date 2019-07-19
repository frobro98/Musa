#pragma once

#include "Importers/TextureImporter.hpp"
#include "ImageFormats.h"

class JPEGImporter : public TextureImporter
{
public:
	JPEGImporter();

// 	JPEGImporter(const JPEGImporter&) = delete;
// 	JPEGImporter(JPEGImporter&&) = delete;
// 	JPEGImporter& operator=(const JPEGImporter&) = delete;

	virtual void SetImportData(const DynamicArray<uint8>& compressedData) override;

private:
	virtual void ProcessImport() override;

private:
	int32 numComponents;
	uint32 pad[1] = { 0 };
};