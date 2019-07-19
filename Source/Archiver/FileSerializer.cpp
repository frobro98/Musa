#include "FileSerializer.hpp"

FileSerializer::FileSerializer(const Path& filePath)
	: file(filePath)
{
	auto result = File::Open(handle, filePath.GetString(), File::Mode::WRITE);
	Assert(result == File::Result::SUCCESS);
}

FileSerializer::~FileSerializer()
{
	Flush();
	auto result = File::Close(handle);
	Assert(result == File::Result::SUCCESS);
}

void FileSerializer::SerializeData(const void* data, uint32 dataSize)
{
	const uint8* byteData = reinterpret_cast<const uint8*>(data);
	for (uint32 i = 0; i < dataSize; ++i)
	{
		serializedData.Add(byteData[i]);
	}
}

void FileSerializer::Flush()
{
	auto result = File::Write(handle, &serializedData[bufferWriteIndex], serializedData.Size() - bufferWriteIndex);
	Assert(result == File::Result::SUCCESS);
	bufferWriteIndex = serializedData.Size();
}
