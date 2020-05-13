// Copyright 2020, Nathan Blane

#include "FileDeserializer.hpp"
#include "MemoryUtilities.h"

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

void FileDeserializer::DeserializeData(void* data, uint64 dataSize)
{
	// TODO - Doesn't really work for things over 4Gb. Brainstorm ways to fix this
	Assert(dataSize <= fileData.Size() - bufferReadLoc);
	Memcpy(data, dataSize, &fileData[bufferReadLoc], dataSize);
	bufferReadLoc += (uint32)dataSize;
}

void FileDeserializer::CacheFile()
{
	uint32 fileSize;
	File::Size(handle, fileSize);
	fileData.Resize(fileSize);

	File::Read(handle, fileData.GetData(), fileSize);
}
