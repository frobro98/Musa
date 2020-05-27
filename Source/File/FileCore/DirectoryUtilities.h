// Copyright 2020, Nathan Blane

#pragma once

#include "String/String.h"
#include "File/FileAPI.hpp"

struct DirectoryDescription
{
	DynamicArray<String> files;
	u32 numberOfFiles;
};

// TODO - Come back to these functions. I think that some of the parameters don't necessarily make sense
// TODO - Move finding directory files somewhere that makes more sense than just in File.h
FILE_API u32 NumberOfFilesInDirectory(const char* directory);
FILE_API u32 GetRequestedNumFiles(const char* directory, DynamicArray<String>& directoryFiles, u32 numFilesRequested);
FILE_API DirectoryDescription GetAllFilesInDirectory(const char* directory);


FILE_API u32 NumberOfFileTypesInCurrentDirectory(const char* fileType);
FILE_API DirectoryDescription GetAllFileTypesInCurrentDirectory(const char* fileType);