// Copyright 2020, Nathan Blane

#pragma once

#include "Importers/TextureImporter.hpp"

class BMPImporter : public TextureImporter
{
public:

	virtual void SetImportData(MemoryBuffer&& compressedData) override;

private:
	virtual void ProcessImport() override;
	void ProcessBMPHeader();

private:
	uint32 pixelDataOffset;

	// Data that will most likely not be here but in the base class
	int32 bitDepth;
	int32 channels;
};
