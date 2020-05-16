// Copyright 2020, Nathan Blane

#pragma once

#include "Path/Path.hpp"
#include "File/FileCore/File.h"
#include "Containers/MemoryBuffer.hpp"
#include "Serialization/DeserializeBase.hpp"

class FileDeserializer final : public DeserializeBase
{
public:
	FileDeserializer(const Path& filePath);
	~FileDeserializer();

	virtual void DeserializeData(void* data, size_t dataSize) override final;

	inline size_t TotalSize() const { return fileData.Size(); }

private:
	void CacheFile();

private:
	MemoryBuffer fileData;
	File::Handle handle;
	uint32 bufferReadLoc = 0;
};