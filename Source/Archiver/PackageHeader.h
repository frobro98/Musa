// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "Archiver/FileSerializer.hpp"
#include "Archiver/FileDeserializer.hpp"

constexpr uint32 PackageVersionSize = 64;
constexpr uint32 PackageNameSize = 64;

struct PackageHeader
{
	char	packageName[PackageNameSize];
	char	versionString[PackageVersionSize];
	uint32	numChunks;
	uint32	totalSize;   // size of file (without package header)
};

forceinline void Deserialize(DeserializeBase& ser, PackageHeader& header)
{
	ser.DeserializeData(header.packageName, PackageNameSize);
	ser.DeserializeData(header.versionString, PackageVersionSize);
	Deserialize(ser, header.numChunks);
	Deserialize(ser, header.totalSize);
}
