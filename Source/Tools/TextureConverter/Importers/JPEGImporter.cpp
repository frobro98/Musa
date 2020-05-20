// Copyright 2020, Nathan Blane

#include "Platform.h"
WALL_WRN_PUSH
#include "libJPEG/jpgd.h"
WALL_WRN_POP

#include "JPEGImporter.hpp"

JPEGImporter::JPEGImporter()
{
}

void JPEGImporter::SetImportData(MemoryBuffer&& data)
{
	importData = std::move(data);
	jpgd::jpeg_decoder_mem_stream jpegMemory(importData.GetData(), (uint32)importData.Size());
	jpgd::jpeg_decoder decoder(&jpegMemory);

	validData = decoder.get_error_code() == jpgd::JPGD_SUCCESS;

	width = decoder.get_width();
	height = decoder.get_height();
	numComponents = decoder.get_num_components();
	if (numComponents == 3)
	{
		format = ImageFormat::RGBA_8u;
		numComponents = 4;
	}
	else if (numComponents == 1)
	{
		format = ImageFormat::Gray_8u;
		numComponents = 1;
	}
}

void JPEGImporter::ProcessImport()
{
	int32 numColors;
	uint8* data = jpgd::decompress_jpeg_image_from_memory(importData.GetData(), static_cast<int32>(importData.Size()), &width, &height, &numColors, numComponents);

	if (data)
	{
		uint32 uncompressedSize = static_cast<uint32>(width * height * numComponents);
		importedImageData.Add(data, uncompressedSize);
		delete[] data;
		data = nullptr;
	}

	validData = data != nullptr;
}
