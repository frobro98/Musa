#pragma once

#include "Platform.h"

// TODO - The file pointer data needs to be cleaned up/moved to String and Array
struct DirectoryDescription
{
	char** files;
	uint32 numberOfFiles;
};

// TODO - Come back to these functions. I think that some of the parameters don't necessarily make sense
// TODO - Move finding directory files somewhere that makes more sense than just in File.h
uint32 NumberOfFilesInDirectory(const char* directory);
uint32 GetRequestedNumFiles(const char* directory, char** directoryFiles, uint32 numFilesRequested);
DirectoryDescription GetAllFilesInDirectory(char* directory);

// TODO - fix the behavior to reflect CurrentDirectory vs Other directory
uint32 NumberOfFileTypesInCurrentDirectory(const char* fileType);
//uint32 GetRequestedNumFileTypes(const char* directory, const char* fileType, char** directoryFiles, uint32 numFilesRequested);
DirectoryDescription GetAllFileTypesInCurrentDirectory(const char* fileType);