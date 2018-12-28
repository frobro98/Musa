#pragma once

WALL_WRN_PUSH
#include "libPNG/png.h"
#include "libPNG/pnginfo.h"
WALL_WRN_POP

#include "Importers/TextureImporter.hpp"

class PNGImporter : public TextureImporter
{
public:
	virtual void SetImportData(const Array<uint8>& compressedData) override;

private:
	static void PNGReadFunc(png_structp png_ptr, png_bytep data, png_size_t length);

private:
	void ProcessPNGHeader();
	virtual void ProcessImport() override;
	bool CheckPNGSignature() const;

private:
	uint32 bufferReadLocation = 0;
	uint32 bitDepth;
	uint32 colorType;
	bool tRNSAlphaExists;
	bool pad[3];
};