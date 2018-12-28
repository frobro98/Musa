#include "FileDeserializer.hpp"
#include "MemoryUtilities.h"

FileDeserializer::FileDeserializer(const Path& filePath)
{
	auto result = File::Open(handle, filePath.GetString(), File::Mode::READ);
	assert(result == File::Result::SUCCESS);
	CacheFile();
}

FileDeserializer::~FileDeserializer()
{
	File::Close(handle);
}

void FileDeserializer::DeserializeData(void* data, uint32 dataSize)
{
	Assert(dataSize <= fileData.Size() - bufferReadLoc);
	Memcpy(data, dataSize, &fileData[bufferReadLoc], dataSize);
	bufferReadLoc += dataSize;
}

void FileDeserializer::CacheFile()
{
	uint32 fileSize;
	File::Size(handle, fileSize);
	fileData.Resize(fileSize);

	File::Read(handle, fileData.GetData(), fileSize);
}
