#include "FileUtilities.hpp"
#include "FileCore/File.h"

DynamicArray<uint8> LoadFileToMemory(const tchar* filePath)
{
	File::Handle handle;
	File::Result result = File::Open(handle, filePath, File::Mode::READ);
	Assert(result == File::Result::SUCCESS);

	uint32 fileSize;
	File::Size(handle, fileSize);
	DynamicArray<uint8> fileData(fileSize);
	result = File::Read(handle, fileData.GetData(), fileSize);
	Assert(result == File::Result::SUCCESS);

	result = File::Close(handle);
	Assert(result == File::Result::SUCCESS);

	return fileData;
}
