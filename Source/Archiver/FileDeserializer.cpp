// Copyright 2020, Nathan Blane

#include "FileDeserializer.hpp"
#include "Utilities/MemoryUtilities.hpp"

FileDeserializer::FileDeserializer(const Path& filePath)
{
	auto result = File::Open(handle, filePath.GetString(), File::Mode::READ);
	Assert(result == File::Result::SUCCESS);
	if (result != File::Result::SUCCESS)
	{
		Assert(false);
	}
	CacheFile();
}

FileDeserializer::~FileDeserializer()
{
	File::Close(handle);
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

	File::Read(handle, fileData.GetData(), fileSize);
}
