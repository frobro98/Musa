// Copyright 2020, Nathan Blane

#pragma once

#include "Platform.h"
#include "ChunkHeader.h"
#include "Archiver/ArchiverDll.hpp"

ARCHIVE_API bool ProcessPackage(
	const char * const filePath,
	Chunk type,
	const char * const chunkName,
	uint8* &chunkBuff,
	uint32& chunkSize,
	uint32& hash
);

