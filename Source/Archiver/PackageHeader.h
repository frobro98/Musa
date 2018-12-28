#pragma once

#include "Platform.h"

constexpr const unsigned int PackageVersionSize = 64;
constexpr const unsigned int PackageNameSize = 64;

struct PackageHeader
{
	char	packageName[PackageNameSize];
	char	versionString[PackageVersionSize];
	uint32	numChunks;
	uint32	totalSize;   // size of file (without package header)
};
