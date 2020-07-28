// Copyright 2020, Nathan Blane

#include "FileSerializer.hpp"
#include "Logging/CoreLogChannels.hpp"
#include "Logging/LogFunctions.hpp"

FileSerializer::FileSerializer(const Path& filePath)
	: pathToFile(filePath)
{
	auto result = File::Open(handle, pathToFile.GetString(), FileMode::Write);
	if (result != FileResult::Success)
	{
		// TODO - GetLastError
		MUSA_ERR(SerializationLog, "Failed to open file {}", *pathToFile.GetFileName());
	}
}

FileSerializer::~FileSerializer()
{
	Flush();
	auto result = File::Close(handle);
	if (result != FileResult::Success)
	{
		// TODO - GetLastError
		MUSA_ERR(SerializationLog, "Failed to close file {}", *pathToFile.GetFileName());
	}
}

void FileSerializer::SerializeData(const void* data, size_t dataSize)
{
	serializedData.Add(data, dataSize);
}

void FileSerializer::Flush()
{
	// TODO - Using the low level file writing interface. Should consider not doing this sort of thing because of the limitations for loading large files
	auto result = File::Write(handle, serializedData.Offset(bufferWriteIndex), (u32)serializedData.Size() - bufferWriteIndex);
	if (result != FileResult::Success)
	{
		// TODO - GetLastError
		MUSA_ERR(SerializationLog, "Failed to write to file {}", *pathToFile.GetFileName());
	}

	bufferWriteIndex = (u32)serializedData.Size();
}
