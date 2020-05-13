// Copyright 2020, Nathan Blane

#pragma once

#include "Path/Path.hpp"
#include "File/FileCore/File.h"
#include "Containers/DynamicArray.hpp"
#include "Serialization/DeserializeBase.hpp"

class FileDeserializer final : public DeserializeBase
{
public:
	FileDeserializer(const Path& filePath);
	~FileDeserializer();

	virtual void DeserializeData(void* data, uint64 dataSize) override final;

	inline uint32 TotalSize() const { return fileData.Size(); }

private:
	void CacheFile();

private:
	DynamicArray<uint8> fileData;
	File::Handle handle;
	uint32 bufferReadLoc = 0;
};