// Copyright 2020, Nathan Blane

#include "FileDeserializer.hpp"
#include "Utilities/MemoryUtilities.hpp"
#include "Logging/CoreLogChannels.hpp"
#include "Logging/LogFunctions.hpp"

FileDeserializer::FileDeserializer(const Path& filePath)
	: pathToFile(filePath)
{
	auto result = File::Open(handle, pathToFile.GetString(), FileMode::Read);
	if (result != FileResult::Success)
	{
		// TODO - Logging
		MUSA_ERR(DeserializationLog, "Failed to open file {}", *pathToFile.GetFileName());
	}
	CacheFile();
}

FileDeserializer::~FileDeserializer()
{
	auto result = File::Close(handle);
	if (result != FileResult::Success)
	{
		// TODO - GetLastError
		MUSA_ERR(DeserializationLog, "Failed to close file {}", *pathToFile.GetFileName());
	}
}

void FileDeserializer::DeserializeData(void* data, size_t dataSize)
{
	// TODO - Doesn't really work for things over 4Gb. Brainstorm ways to fix this
	Assert(dataSize <= fileData.Size() - bufferReadLoc);
	Memcpy(data, dataSize, fileData.Offset(bufferReadLoc), dataSize);
	bufferReadLoc += (u32)dataSize;
}

void FileDeserializer::CacheFile()
{
	u32 fileSize;
	File::Size(handle, fileSize);
	fileData.IncreaseSize(fileSize);

	FileResult result = File::Read(handle, fileData.GetData(), fileSize);
	if (result != FileResult::Success)
	{
		// TODO - GetLastError
		MUSA_ERR(DeserializationLog, "Failed to read from file {}", *pathToFile.GetFileName());
	}
}
