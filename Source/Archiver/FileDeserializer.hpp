#pragma once

#include "File/Path.hpp"
#include "File/FileCore/File.h"
#include "Containers/Array.h"
#include "Serialization/DeserializeBase.hpp"

class FileDeserializer final : public DeserializeBase
{
public:
	FileDeserializer(const Path& filePath);
	~FileDeserializer();

	virtual void DeserializeData(void* data, uint32 dataSize) override final;

	inline uint32 TotalSize() const { return fileData.Size(); }

private:
	void CacheFile();

private:
	Array<uint8> fileData;
	uint32 bufferReadLoc = 0;
	File::Handle handle;
};