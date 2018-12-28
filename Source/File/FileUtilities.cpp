#include "FileUtilities.hpp"
#include "FileCore/File.h"

Array<uint8> LoadFileToMemory(const Path& filePath)
{
	File::Handle handle;
	File::Result result = File::Open(handle, filePath.GetString(), File::Mode::READ);
	Assert(result == File::Result::SUCCESS);

	uint32 fileSize;
	File::Size(handle, fileSize);
	Array<uint8> fileData(fileSize);
	result = File::Read(handle, fileData.GetData(), fileSize);
	Assert(result == File::Result::SUCCESS);

	result = File::Close(handle);
	Assert(result == File::Result::SUCCESS);

	return fileData;
}
