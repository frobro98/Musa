// Copyright 2020, Nathan Blane

#pragma once

#include "Platform.h"
#include "String/String.h"

struct DirectoryDescription
{
	DynamicArray<String> files;
	uint32 numberOfFiles;
	uint32 pad[1] = { 0 };
};

// TODO - Come back to these functions. I think that some of the parameters don't necessarily make sense
// TODO - Move finding directory files somewhere that makes more sense than just in File.h
uint32 NumberOfFilesInDirectory(const char* directory);
uint32 GetRequestedNumFiles(const char* directory, DynamicArray<String>& directoryFiles, uint32 numFilesRequested);
DirectoryDescription GetAllFilesInDirectory(const char* directory);

uint32 NumberOfFileTypesInCurrentDirectory(const char* fileType);
DirectoryDescription GetAllFileTypesInCurrentDirectory(const char* fileType);