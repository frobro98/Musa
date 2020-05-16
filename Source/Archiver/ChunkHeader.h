// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "Archiver/FileSerializer.hpp"
#include "Archiver/FileDeserializer.hpp"

enum class Chunk
{
	VERTS_TYPE,
	WEIGHTS_TYPE,
	SKEL_TYPE,
	ANIM_TYPE,
	TEXTURE_TYPE
};

constexpr uint32 ChunkNameSize = 64;

struct ChunkHeader
{
	char     chunkName[ChunkNameSize];
	uint32   chunkSize;
	uint32   hashNum;
	Chunk	 type;
};

forceinline void Deserialize(DeserializeBase& ser, ChunkHeader& header)
{
	ser.DeserializeData(header.chunkName, ChunkNameSize);
	Deserialize(ser, header.chunkSize);
	Deserialize(ser, header.hashNum);
	uint32 e;
	Deserialize(ser, e);
	// TODO - There needs to be some sort of serialization scheme for enum types
	header.type = (Chunk)e;
}
