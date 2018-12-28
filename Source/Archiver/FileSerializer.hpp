#pragma once

#include "File/Path.hpp"
#include "File/FileCore/File.h"
#include "Containers/Array.h"
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
	Array<uint8> serializedData;
	uint32 bufferWriteIndex = 0;
	File::Handle handle;
};