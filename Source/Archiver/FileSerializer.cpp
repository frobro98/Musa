// Copyright 2020, Nathan Blane

#include "FileSerializer.hpp"

FileSerializer::FileSerializer(const Path& filePath)
{
	auto result = File::Open(handle, filePath.GetString(), File::Mode::WRITE);
	Assert(result == File::Result::SUCCESS);
	if (result != File::Result::SUCCESS)
	{

	}
}

FileSerializer::~FileSerializer()
{
	Flush();
	auto result = File::Close(handle);
	Assert(result == File::Result::SUCCESS);
	if (result != File::Result::SUCCESS)
	{
		Assert(false);
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
	Assert(result == File::Result::SUCCESS);
	if (result != File::Result::SUCCESS)
	{
		Assert(false);
	}

	bufferWriteIndex = (u32)serializedData.Size();
}
