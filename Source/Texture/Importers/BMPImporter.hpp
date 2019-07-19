#pragma once

#include "Importers/TextureImporter.hpp"

class BMPImporter : public TextureImporter
{
public:

	virtual void SetImportData(const DynamicArray<uint8>& compressedData) override;

private:
	virtual void ProcessImport() override;
	void ProcessBMPHeader();

private:
	uint32 pixelDataOffset;

	// Data that will most likely not be here but in the base class
	int32 bitDepth;
	int32 channels;
	uint32 pad[1] = { 0 };
};
