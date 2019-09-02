#pragma once

#include "Path/Path.hpp"
#include "File/FileCore/File.h"
#include "Containers/DynamicArray.hpp"
#include "Serialization/SerializeBase.hpp"

class FileSerializer final : public SerializeBase
{
public:
	FileSerializer(const Path& filePath);
	~FileSerializer();

	virtual void SerializeData(const void* data, uint32 dataSize) override final;

	inline uint32 TotalSize() { return serializedData.Size(); }

private:
	void Flush();

private:
	Path file;
	DynamicArray<uint8> serializedData;
	File::Handle handle;
	uint32 bufferWriteIndex = 0;
	uint32 pad[1] = { 0 };
};