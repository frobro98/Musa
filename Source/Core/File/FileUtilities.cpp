// Copyright 2020, Nathan Blane

#include "Debugging/Assertion.hpp"
#include "FileUtilities.hpp"
#include "FileSys.hpp"

MemoryBuffer LoadFileToMemory(const tchar* filePath)
{
	File::Handle handle;
	FileResult result = File::Open(handle, filePath, FileMode::Read);
	Assert(result == FileResult::Success);

	u32 fileSize;
	File::Size(handle, fileSize);
	MemoryBuffer fileData(fileSize);
	result = File::Read(handle, fileData.GetData(), fileSize);
	Assert(result == FileResult::Success);

	result = File::Close(handle);
	Assert(result == FileResult::Success);

	return fileData;
}
