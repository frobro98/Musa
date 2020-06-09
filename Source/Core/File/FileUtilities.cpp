// Copyright 2020, Nathan Blane

#include "Debugging/Assertion.hpp"
#include "FileUtilities.hpp"
#include "FileSys.hpp"

MemoryBuffer LoadFileToMemory(const tchar* filePath)
{
	File::Handle handle;
	File::Result result = File::Open(handle, filePath, File::Mode::READ);
	Assert(result == File::Result::SUCCESS);

	u32 fileSize;
	File::Size(handle, fileSize);
	MemoryBuffer fileData(fileSize);
	result = File::Read(handle, fileData.GetData(), fileSize);
	Assert(result == File::Result::SUCCESS);

	result = File::Close(handle);
	Assert(result == File::Result::SUCCESS);

	return fileData;
}
